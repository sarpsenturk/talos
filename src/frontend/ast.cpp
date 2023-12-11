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
} // namespace talos
