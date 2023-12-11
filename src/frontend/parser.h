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
        SourceLocation location;
    };

    using ParserResult = expected<ExprPtr, ParserError>;
    using ExprResult = expected<ExprPtr, ParserError>;

    class Parser
    {
    public:
        explicit Parser(Lexer* lexer);

        ParserResult parse();

    private:
        ExprResult expression();
        ExprResult additive_expr();
        ExprResult factor_expr();
        ExprResult unary_expr();
        ExprResult literal_expr();

        void consume_token();
        bool expect_and_consume(std::span<const TokenType> expected);
        bool expect_and_consume(TokenType expected);
        ParserResult syntax_error(std::string message) const;

        Lexer* lexer_;
        Token current_token_;
        Token next_token_;
    };
} // namespace talos
