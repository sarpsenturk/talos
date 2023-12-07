#pragma once

#include "ast.h"
#include "expected.h"
#include "lexer.h"

#include <span>
#include <string>

namespace talos
{
    struct ParserError {
        ReturnCode code = ReturnCode::SyntaxError;
        std::string message;
    };

    using ParserResult = expected<ExprPtr, ParserError>;

    class Parser
    {
    public:
        explicit Parser(Lexer* lexer);

        ParserResult parse();

    private:
        ParserResult expression();
        ParserResult additive_expr();
        ParserResult factor_expr();
        ParserResult literal_expr();

        void consume_token();
        bool expect_and_consume(std::span<const TokenType> expected);
        bool expect_and_consume(TokenType expected);

        Lexer* lexer_;
        Token current_token_;
        Token next_token_;
    };
} // namespace talos
