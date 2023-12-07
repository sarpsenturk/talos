#pragma once

#include "token.h"

#include <memory>

namespace talos
{
    class Expr;
    class BinaryExpr;
    class ParenExpr;
    class IntLiteralExpr;

    using ExprPtr = std::unique_ptr<Expr>;

    class ExprVisitor
    {
    public:
        ExprVisitor() = default;
        virtual ~ExprVisitor() = default;

        virtual void visit(const BinaryExpr& expr) = 0;
        virtual void visit(const ParenExpr& expr) = 0;
        virtual void visit(const IntLiteralExpr& expr) = 0;

    protected:
        ExprVisitor(const ExprVisitor&) = default;
        ExprVisitor(ExprVisitor&&) noexcept = default;
        ExprVisitor& operator=(const ExprVisitor&) = default;
        ExprVisitor& operator=(ExprVisitor&&) noexcept = default;
    };

    class Expr
    {
    public:
        Expr() = default;
        virtual ~Expr() = default;

        virtual void visit(ExprVisitor&) = 0;

    protected:
        Expr(const Expr&) = default;
        Expr(Expr&&) noexcept = default;
        Expr& operator=(const Expr&) = default;
        Expr& operator=(Expr&&) noexcept = default;
    };

    class BinaryExpr : public Expr
    {
    public:
        BinaryExpr(ExprPtr lhs, Token op, ExprPtr rhs);

        [[nodiscard]] const Expr* lhs() const noexcept { return lhs_.get(); }
        [[nodiscard]] Token op() const noexcept { return op_; }
        [[nodiscard]] const Expr* rhs() const noexcept { return rhs_.get(); }

        void visit(ExprVisitor& visitor) override { visitor.visit(*this); }

    private:
        ExprPtr lhs_;
        Token op_;
        ExprPtr rhs_;
    };

    class ParenExpr : public Expr
    {
    public:
        explicit ParenExpr(ExprPtr expr);

        [[nodiscard]] const Expr* expr() const noexcept { return expr_.get(); }

        void visit(ExprVisitor& visitor) override { visitor.visit(*this); }

    private:
        ExprPtr expr_;
    };

    class IntLiteralExpr : public Expr
    {
    public:
        explicit IntLiteralExpr(Token int_token);

        [[nodiscard]] Token int_token() const noexcept { return int_token_; }

        void visit(ExprVisitor& visitor) override { visitor.visit(*this); }

    private:
        Token int_token_;
    };
} // namespace talos
