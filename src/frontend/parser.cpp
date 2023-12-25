#include "parser.h"

#include <algorithm>

#include <fmt/ranges.h>

namespace talos
{
    namespace
    {
        constexpr UnexpectedToken to_token_error(const LexerError& error)
        {
            return {error.message, error.location, error.code};
        }

        constexpr ParserError to_parser_error(const LexerError& error)
        {
            return {.code = error.code, .message = error.message, .location = error.location};
        }

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

    ParserResult Parser::parse()
    {
        consume_token();
        std::vector<StatementPtr> statements;
        while (!is_eof()) {
            auto stmt = declaration();
            if (!stmt) {
                return unexpected(stmt.error());
            }
            statements.push_back(std::move(*stmt));
        }
        return ProgramNode{std::move(statements)};
    }

    StmtResult Parser::declaration()
    {
        if (expect_and_consume({{TokenType::Var, TokenType::Let}})) {
            return var_decl();
        }
        if (expect_and_consume(TokenType::Fun)) {
            return fun_decl();
        }
        return statement();
    }

    StmtResult Parser::var_decl()
    {
        // 'var' or 'let'
        auto decl_type = current_token_;

        auto identifier = expect_and_consume(TokenType::Identifier);
        if (!identifier) {
            return syntax_error("Expected variable identifier");
        }

        std::optional<Token> type_spec;
        if (expect_and_consume(TokenType::Colon)) {
            auto token = consume_if(is_type_specifier);
            if (!token) {
                return unexpected(to_parser_error(token.error()));
            }
            if (!token->has_value()) {
                return syntax_error("Expected type specifier after ':'");
            }
            type_spec = *token;
        }

        if (!expect_and_consume(TokenType::Equal)) {
            return syntax_error("Expected '=' after identifier");
        }

        auto value = expression();
        if (!value) {
            return unexpected(value.error());
        }

        if (!expect_and_consume(TokenType::Semicolon)) {
            return syntax_error("Expected ';' after variable");
        }

        return std::make_unique<VarDeclStatement>(decl_type, *identifier, type_spec, std::move(*value));
    }

    StmtResult Parser::fun_decl()
    {
        auto identifier = expect_and_consume(TokenType::Identifier);
        if (!identifier) {
            return syntax_error("Expected function identifier after fun");
        }

        if (!expect_and_consume(TokenType::LeftParen)) {
            return syntax_error("Expected '(' after function name");
        }
        // TODO: Function parameters
        if (!expect_and_consume(TokenType::RightParen)) {
            return syntax_error("Expected ')' after parameter list");
        }

        std::optional<Token> type_spec;
        if (expect_and_consume(TokenType::Colon)) {
            auto token = consume_if(is_type_specifier);
            if (!token) {
                return unexpected(to_parser_error(token.error()));
            }
            if (!token->has_value()) {
                return syntax_error("Expected type specifier after ':'");
            }
            type_spec = *token;
        }

        StatementList statements;
        if (!expect_and_consume(TokenType::LeftBrace)) {
            return syntax_error("Expected '{' to begin function block");
        }

        while (!expect_and_consume(TokenType::RightBrace)) {
            if (is_eof()) {
                return syntax_error("Unexpected EOF. Expected '}' to end function block");
            }
            auto stmt = declaration();
            if (!stmt) {
                return stmt;
            }
            statements.push_back(std::move(*stmt));
        }
        return std::make_unique<FunDeclStatement>(*identifier, type_spec, std::move(statements));
    }

    StmtResult Parser::statement()
    {
        if (expect_and_consume(TokenType::Return)) {
            return return_statement();
        }
        return expr_statement();
    }

    StmtResult Parser::return_statement()
    {
        auto return_value = expression();
        if (!return_value) {
            return unexpected(return_value.error());
        }

        if (!expect_and_consume(TokenType::Semicolon)) {
            return syntax_error("Expected ';' after return statement");
        }

        return std::make_unique<ReturnStatement>(std::move(*return_value));
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
        return assignment_expr();
    }

    ExprResult Parser::assignment_expr()
    {
        auto expr = additive_expr();
        if (!expr) {
            return expr;
        }
        while (expect_and_consume(TokenType::Equal)) {
            auto rhs = assignment_expr();
            if (!rhs) {
                return rhs;
            }
            expr = std::make_unique<AssignmentExpr>(std::move(*expr), std::move(*rhs));
        }
        return expr;
    }

    ExprResult Parser::additive_expr()
    {
        auto expr = factor_expr();
        if (!expr) {
            return expr;
        }
        while (auto binary_op = expect_and_consume({{TokenType::Plus, TokenType::Minus}})) {
            auto rhs = factor_expr();
            if (!rhs) {
                return rhs;
            }
            expr = std::make_unique<BinaryExpr>(std::move(*expr), *binary_op, std::move(*rhs));
        }
        return expr;
    }

    ExprResult Parser::factor_expr()
    {
        auto expr = unary_expr();
        if (!expr) {
            return expr;
        }
        while (auto binary_op = expect_and_consume({{TokenType::Star, TokenType::Slash}})) {
            auto rhs = unary_expr();
            if (!rhs) {
                return rhs;
            }
            expr = std::make_unique<BinaryExpr>(std::move(*expr), *binary_op, std::move(*rhs));
        }
        return expr;
    }

    ExprResult Parser::unary_expr()
    {
        if (auto unary_op = expect_and_consume({{TokenType::Minus}})) {
            auto expr = unary_expr();
            if (expr) {
                return std::make_unique<UnaryExpr>(*unary_op, std::move(*expr));
            }
            return expr;
        }
        return literal_expr();
    }

    ExprResult Parser::literal_expr()
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
        if (auto identifier = expect_and_consume(TokenType::Identifier)) {
            return std::make_unique<IdentifierExpr>(*identifier);
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

    LexerReturn Parser::consume_token()
    {
        current_token_ = next_token_;
        auto next = lexer_->consume_token();
        if (!next) {
            return next;
        }
        next_token_ = *next;
        return current_token_;
    }

    ExpectTokenResult Parser::expect_and_consume(std::span<const TokenType> expected)
    {
        if (const auto iter = std::ranges::find(expected, next_token_.type); iter != expected.end()) {
            auto token = consume_token();
            if (!token) {
                return unexpected(to_token_error(token.error()));
            }
            return *token;
        }
        return unexpected(UnexpectedToken{
            .msg = fmt::format("Expected token {}, got {}", expected, next_token_.type),
            .location = next_token_.location,
            .code = ReturnCode::UnexpectedToken,
        });
    }

    ExpectTokenResult Parser::expect_and_consume(TokenType expected)
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
