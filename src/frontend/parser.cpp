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
        return expression();
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
        auto expr = literal_expr();
        if (!expr) {
            return expr;
        }
        while (expect_and_consume({{TokenType::Star, TokenType::Slash}})) {
            auto op = current_token_;
            auto rhs = literal_expr();
            if (!rhs) {
                return rhs;
            }
            expr = std::make_unique<BinaryExpr>(std::move(*expr), op, std::move(*rhs));
        }
        return expr;
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

    void Parser::consume_token()
    {
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

    ParserResult Parser::syntax_error(std::string message) const
    {
        return unexpected(ParserError{
            .message = std::move(message),
            .location = current_token_.location,
        });
    }
} // namespace talos
