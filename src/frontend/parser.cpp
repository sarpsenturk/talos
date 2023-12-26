#include "parser.h"

#include "exceptions.h"

#include <algorithm>

#include <fmt/ranges.h>

namespace talos
{
    namespace
    {
        constexpr bool is_type_specifier(const Token& token)
        {
            return std::ranges::any_of(type_specifier_tokens, [expected = token.type](auto type) {
                return type == expected;
            });
        }
    } // namespace

    Parser::Parser(Lexer* lexer)
        : lexer_(lexer)
    {
    }

    ProgramNode Parser::parse()
    {
        consume_token();
        std::vector<StatementPtr> statements;
        while (!is_eof()) {
            statements.push_back(declaration());
        }
        return ProgramNode{std::move(statements)};
    }

    std::unique_ptr<Statement> Parser::declaration()
    {
        if (expect_and_consume({{TokenType::Var, TokenType::Let}})) {
            return var_decl();
        }
        if (expect_and_consume(TokenType::Fun)) {
            return fun_decl();
        }
        return statement();
    }

    std::unique_ptr<Statement> Parser::var_decl()
    {
        // 'var' or 'let'
        auto decl_type = current_token_;

        auto identifier = expect_and_consume(TokenType::Identifier);
        if (!identifier) {
            throw syntax_error(location(), "Expected variable identifier");
        }

        std::optional<Token> type_spec;
        if (expect_and_consume(TokenType::Colon)) {
            type_spec = consume_if(is_type_specifier);
            if (!type_spec.has_value()) {
                throw syntax_error(location(), "Expected type specifier after ':'");
            }
        }

        if (!expect_and_consume(TokenType::Equal)) {
            throw syntax_error(location(), "Expected '=' after identifier");
        }

        auto value = expression();
        if (!expect_and_consume(TokenType::Semicolon)) {
            throw syntax_error(location(), "Expected ';' after variable");
        }

        return std::make_unique<VarDeclStatement>(decl_type, *identifier, type_spec, std::move(value));
    }

    std::unique_ptr<Statement> Parser::fun_decl()
    {
        auto identifier = expect_and_consume(TokenType::Identifier);
        if (!identifier) {
            throw syntax_error(location(), "Expected function identifier after fun");
        }

        if (!expect_and_consume(TokenType::LeftParen)) {
            throw syntax_error(location(), "Expected '(' after function name");
        }
        // TODO: Function parameters
        if (!expect_and_consume(TokenType::RightParen)) {
            throw syntax_error(location(), "Expected ')' after parameter list");
        }

        std::optional<Token> type_spec;
        if (expect_and_consume(TokenType::Colon)) {
            type_spec = consume_if(is_type_specifier);
            if (!type_spec.has_value()) {
                throw syntax_error(location(), "Expected type specifier after ':'");
            }
        }

        StatementList statements;
        if (!expect_and_consume(TokenType::LeftBrace)) {
            throw syntax_error(location(), "Expected '{' to begin function block");
        }

        while (!expect_and_consume(TokenType::RightBrace)) {
            if (is_eof()) {
                throw unexpected_eof(location(), "Unexpected EOF. Expected '}' to end function block");
            }
            statements.push_back(declaration());
        }
        return std::make_unique<FunDeclStatement>(*identifier, type_spec, std::move(statements));
    }

    std::unique_ptr<Statement> Parser::statement()
    {
        if (expect_and_consume(TokenType::Return)) {
            return return_statement();
        }
        return expr_statement();
    }

    std::unique_ptr<Statement> Parser::return_statement()
    {
        auto return_value = expression();
        if (!expect_and_consume(TokenType::Semicolon)) {
            throw syntax_error(location(), "Expected ';' after return statement");
        }

        return std::make_unique<ReturnStatement>(std::move(return_value));
    }

    std::unique_ptr<Statement> Parser::expr_statement()
    {
        auto expr = expression();
        if (!expect_and_consume(TokenType::Semicolon)) {
            throw syntax_error(location(), "Expected ';' after statement");
        }
        return std::make_unique<ExprStatement>(std::move(expr));
    }

    std::unique_ptr<Expr> Parser::expression()
    {
        return assignment_expr();
    }

    std::unique_ptr<Expr> Parser::assignment_expr()
    {
        auto expr = additive_expr();
        while (expect_and_consume(TokenType::Equal)) {
            expr = std::make_unique<AssignmentExpr>(std::move(expr), assignment_expr());
        }
        return expr;
    }

    std::unique_ptr<Expr> Parser::additive_expr()
    {
        auto expr = factor_expr();
        while (auto binary_op = expect_and_consume({{TokenType::Plus, TokenType::Minus}})) {
            expr = std::make_unique<BinaryExpr>(std::move(expr), *binary_op, factor_expr());
        }
        return expr;
    }

    std::unique_ptr<Expr> Parser::factor_expr()
    {
        auto expr = unary_expr();
        while (auto binary_op = expect_and_consume({{TokenType::Star, TokenType::Slash}})) {
            expr = std::make_unique<BinaryExpr>(std::move(expr), *binary_op, unary_expr());
        }
        return expr;
    }

    std::unique_ptr<Expr> Parser::unary_expr()
    {
        if (auto unary_op = expect_and_consume({{TokenType::Minus}})) {
            return std::make_unique<UnaryExpr>(*unary_op, unary_expr());
        }
        return literal_expr();
    }

    std::unique_ptr<Expr> Parser::literal_expr()
    {
        if (auto integer = expect_and_consume(TokenType::IntLiteral)) {
            return std::make_unique<IntLiteralExpr>(*integer);
        }
        if (auto string = expect_and_consume(TokenType::StringLiteral)) {
            return std::make_unique<StringLiteralExpr>(*string);
        }
        if (auto character = expect_and_consume(TokenType::CharLiteral)) {
            return std::make_unique<CharLiteralExpr>(*character);
        }
        if (auto floating = expect_and_consume(TokenType::FloatLiteral)) {
            return std::make_unique<FloatingLiteralExpr>(*floating);
        }
        if (auto identifier = expect_and_consume(TokenType::Identifier)) {
            return std::make_unique<IdentifierExpr>(*identifier);
        }
        if (expect_and_consume(TokenType::LeftParen)) {
            auto expr = expression();
            if (!expect_and_consume(TokenType::RightParen)) {
                throw syntax_error(location(), "Expected ')' after expression");
            }
            return std::make_unique<ParenExpr>(std::move(expr));
        }
        throw syntax_error(location(), "Expected expression");
    }

    bool Parser::is_eof() const noexcept
    {
        return next_token_.type == TokenType::Eof;
    }

    SourceLocation Parser::location() const noexcept
    {
        return next_token_.location;
    }

    Token Parser::consume_token()
    {
        current_token_ = next_token_;
        next_token_ = lexer_->consume_token();
        return current_token_;
    }

    std::optional<Token> Parser::expect_and_consume(std::span<const TokenType> expected)
    {
        if (const auto iter = std::ranges::find(expected, next_token_.type); iter != expected.end()) {
            return consume_token();
        }
        return std::nullopt;
    }

    std::optional<Token> Parser::expect_and_consume(TokenType expected)
    {
        return expect_and_consume({{expected}});
    }
} // namespace talos
