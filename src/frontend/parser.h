#pragma once

#include "ast.h"
#include "lexer.h"

#include <span>
#include <string>

namespace talos
{
    class Parser
    {
    public:
        explicit Parser(Lexer* lexer);

        ProgramNode parse();

    private:
        std::unique_ptr<Statement> declaration();
        std::unique_ptr<Statement> var_decl();
        std::unique_ptr<Statement> fun_decl();
        std::unique_ptr<Statement> statement();
        std::unique_ptr<Statement> return_statement();
        std::unique_ptr<Statement> expr_statement();
        std::unique_ptr<Expr> expression();
        std::unique_ptr<Expr> assignment_expr();
        std::unique_ptr<Expr> additive_expr();
        std::unique_ptr<Expr> factor_expr();
        std::unique_ptr<Expr> unary_expr();
        std::unique_ptr<Expr> literal_expr();

        [[nodiscard]] bool is_eof() const noexcept;
        [[nodiscard]] SourceLocation location() const noexcept;

        Token consume_token();
        std::optional<Token> expect_and_consume(std::span<const TokenType> expected);
        std::optional<Token> expect_and_consume(TokenType expected);

        template<TokenPredicate F>
        std::optional<Token> consume_if(F callable)
        {
            if (callable(next_token_)) {
                return consume_token();
            }
            return std::nullopt;
        }

        Lexer* lexer_;
        Token current_token_;
        Token next_token_;
    };
} // namespace talos
