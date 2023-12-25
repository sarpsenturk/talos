#include "ast.h"

namespace talos
{
    BinaryExpr::BinaryExpr(ExprPtr lhs, Token op, ExprPtr rhs)
        : lhs_(std::move(lhs))
        , op_(op)
        , rhs_(std::move(rhs))
    {
    }

    UnaryExpr::UnaryExpr(Token unary_op, ExprPtr expr)
        : unary_op_(unary_op)
        , expr_(std::move(expr))
    {
    }

    ParenExpr::ParenExpr(ExprPtr expr)
        : expr_(std::move(expr))
    {
    }

    IntLiteralExpr::IntLiteralExpr(Token int_token)
        : int_token_(int_token)
    {
    }

    StringLiteralExpr::StringLiteralExpr(Token string_literal)
        : string_literal_(string_literal)
    {
    }

    CharLiteralExpr::CharLiteralExpr(Token char_literal)
        : char_literal_(char_literal)
    {
    }

    FloatingLiteralExpr::FloatingLiteralExpr(Token float_literal)
        : float_literal_(float_literal)
    {
    }

    IdentifierExpr::IdentifierExpr(Token identifier)
        : identifier_(identifier)
    {
    }

    AssignmentExpr::AssignmentExpr(ExprPtr lhs, ExprPtr rhs)
        : lhs_(std::move(lhs))
        , rhs_(std::move(rhs))
    // ippek<3
    {
    }

    ExprStatement::ExprStatement(ExprPtr expr)
        : expr_(std::move(expr))
    {
    }

    ReturnStatement::ReturnStatement(ExprPtr return_value)
        : return_value_(std::move(return_value))
    {
    }

    VarDeclStatement::VarDeclStatement(Token decl_type, Token identifier, std::optional<Token> type_spec, ExprPtr initializer)
        : decl_type_(decl_type)
        , identifier_(identifier)
        , type_specifier_(type_spec)
        , initializer_(std::move(initializer))
    {
    }

    FunDeclStatement::FunDeclStatement(Token identifier, std::optional<Token> type_spec, StatementList statements)
        : identifier_(identifier)
        , type_spec_(type_spec)
        , statements_(std::move(statements))
    {
    }

    ProgramNode::ProgramNode(std::vector<StatementPtr> statements)
        : statements_(std::move(statements))
    {
    }
} // namespace talos
