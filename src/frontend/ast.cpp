#include "ast.h"

namespace talos
{
    BinaryExpr::BinaryExpr(ExprPtr lhs, Token op, ExprPtr rhs)
        : lhs_(std::move(lhs))
        , op_(op)
        , rhs_(std::move(rhs))
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
