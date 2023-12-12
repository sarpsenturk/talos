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

    ExprStatement::ExprStatement(ExprPtr expr)
        : expr_(std::move(expr))
    {
    }

    FunStatement::FunStatement(Token identifier, StatementList statements)
        : identifier_(identifier)
        , statements_(std::move(statements))
    {
    }

    ReturnStatement::ReturnStatement(ExprPtr return_value)
        : return_value_(std::move(return_value))
    {
    }

    VarStatement::VarStatement(Token identifier, ExprPtr value)
        : identifier_(identifier)
        , value_(std::move(value))
    {
    }

    ConstStatement::ConstStatement(Token identifier, ExprPtr value)
        : identifier_(identifier)
        , value_(std::move(value))
    {
    }

    ProgramNode::ProgramNode(std::vector<StatementPtr> statements)
        : statements_(std::move(statements))
    {
    }
} // namespace talos
