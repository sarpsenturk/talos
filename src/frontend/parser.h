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

    using ParserResult = expected<ProgramNode, ParserError>;
    using ExprResult = expected<ExprPtr, ParserError>;
    using StmtResult = expected<StatementPtr, ParserError>;

    struct UnexpectedToken {
        std::string msg;
        SourceLocation location;
        ReturnCode code;
    };

    using ExpectTokenResult = expected<Token, UnexpectedToken>;

    class Parser
    {
    public:
        explicit Parser(Lexer* lexer);

        ParserResult parse();

    private:
        StmtResult declaration();
        StmtResult var_decl();
        StmtResult fun_decl();
        StmtResult statement();
        StmtResult return_statement();
        StmtResult expr_statement();
        ExprResult expression();
        ExprResult assignment_expr();
        ExprResult additive_expr();
        ExprResult factor_expr();
        ExprResult unary_expr();
        ExprResult literal_expr();

        [[nodiscard]] bool is_eof() const noexcept;

        LexerReturn consume_token();

        ExpectTokenResult expect_and_consume(std::span<const TokenType> expected);
        ExpectTokenResult expect_and_consume(TokenType expected);

        unexpected<ParserError> syntax_error(std::string message) const;

        Lexer* lexer_;
        Token current_token_;
        Token next_token_;
    };
} // namespace talos
