#include "parser.h"

#include <algorithm>

namespace talos
{
    Parser::Parser(Lexer* lexer)
        : lexer_(lexer)
    {
    }

    ParserResult Parser::parse()
    {
        consume_token();
        std::vector<StatementPtr> statements;
        while (!is_eof()) {
            auto stmt = statement();
            if (!stmt) {
                return unexpected(stmt.error());
            }
            statements.push_back(std::move(*stmt));
        }
        return ProgramNode{std::move(statements)};
    }

    StmtResult Parser::statement()
    {
        return expr_statement();
    }

    StmtResult Parser::expr_statement()
    {
        auto expr = expression();
        if (!expr) {
            return unexpected(expr.error());
        }
        if (!expect_and_consume(TokenType::Semicolon)) {
            return syntax_error("Expected ';' after statement");
        }
        return std::make_unique<ExprStatement>(std::move(*expr));
    }

    ExprResult Parser::expression()
    {
        return additive_expr();
    }

    ExprResult Parser::additive_expr()
    {
        auto expr = factor_expr();
        if (!expr) {
            return expr;
        }
        while (expect_and_consume({{TokenType::Plus, TokenType::Minus}})) {
            auto op = current_token_;
            auto rhs = factor_expr();
            if (!rhs) {
                return rhs;
            }
            expr = std::make_unique<BinaryExpr>(std::move(*expr), op, std::move(*rhs));
        }
        return expr;
    }

    ExprResult Parser::factor_expr()
    {
        auto expr = unary_expr();
        if (!expr) {
            return expr;
        }
        while (expect_and_consume({{TokenType::Star, TokenType::Slash}})) {
            auto op = current_token_;
            auto rhs = unary_expr();
            if (!rhs) {
                return rhs;
            }
            expr = std::make_unique<BinaryExpr>(std::move(*expr), op, std::move(*rhs));
        }
        return expr;
    }

    ExprResult Parser::unary_expr()
    {
        if (expect_and_consume({{TokenType::Minus}})) {
            auto unary_op = current_token_;
            auto expr = unary_expr();
            if (expr) {
                return std::make_unique<UnaryExpr>(unary_op, std::move(*expr));
            }
            return expr;
        }
        return literal_expr();
    }

    ExprResult Parser::literal_expr()
    {
        if (expect_and_consume(TokenType::Integer)) {
            return std::make_unique<IntLiteralExpr>(current_token_);
        }
        if (expect_and_consume(TokenType::LeftParen)) {
            auto expr = expression();
            if (!expect_and_consume(TokenType::RightParen)) {
                return syntax_error("Expected ')' after expression");
            }
            return std::make_unique<ParenExpr>(std::move(*expr));
        }
        return syntax_error("Expected expression");
    }

    bool Parser::is_eof() const noexcept
    {
        return next_token_.type == TokenType::Eof;
    }

    void Parser::consume_token()
    {
        // TODO: We don't check for lex errors here
        //  We should probably do that.
        current_token_ = next_token_;
        next_token_ = lexer_->consume_token().value();
    }

    bool Parser::expect_and_consume(std::span<const TokenType> expected)
    {
        if (const auto iter = std::ranges::find(expected, next_token_.type); iter != expected.end()) {
            consume_token();
            return true;
        }
        return false;
    }

    bool Parser::expect_and_consume(TokenType expected)
    {
        return expect_and_consume({{expected}});
    }

    unexpected<ParserError> Parser::syntax_error(std::string message) const
    {
        return unexpected(ParserError{
            .message = std::move(message),
            .location = current_token_.location,
        });
    }
} // namespace talos
