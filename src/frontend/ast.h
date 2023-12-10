#pragma once

#include "token.h"

#include <memory>

namespace talos
{
    class ASTNode;
    class Expr;
    class BinaryExpr;
    class ParenExpr;
    class IntLiteralExpr;

    using ASTNodePtr = std::unique_ptr<ASTNode>;
    using ExprPtr = std::unique_ptr<Expr>;

    class ASTVisitor
    {
    public:
        ASTVisitor() = default;
        virtual ~ASTVisitor() = default;

        virtual void visit(const BinaryExpr& expr) = 0;
        virtual void visit(const ParenExpr& expr) = 0;
        virtual void visit(const IntLiteralExpr& expr) = 0;

    protected:
        ASTVisitor(const ASTVisitor&) = default;
        ASTVisitor(ASTVisitor&&) noexcept = default;
        ASTVisitor& operator=(const ASTVisitor&) = default;
        ASTVisitor& operator=(ASTVisitor&&) noexcept = default;
    };

    class ASTNode
    {
    public:
        ASTNode() = default;
        virtual ~ASTNode() = default;

        virtual void visit(ASTVisitor&) = 0;

    protected:
        ASTNode(const ASTNode&) = default;
        ASTNode(ASTNode&&) noexcept = default;
        ASTNode& operator=(const ASTNode&) = default;
        ASTNode& operator=(ASTNode&&) noexcept = default;
    };

    class Expr : public ASTNode
    {
    public:
        Expr() = default;
    };

    class BinaryExpr : public Expr
    {
    public:
        BinaryExpr(ExprPtr lhs, Token op, ExprPtr rhs);

        [[nodiscard]] const Expr* lhs() const noexcept { return lhs_.get(); }
        [[nodiscard]] Token op() const noexcept { return op_; }
        [[nodiscard]] const Expr* rhs() const noexcept { return rhs_.get(); }

        void visit(ASTVisitor& visitor) override { visitor.visit(*this); }

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

        void visit(ASTVisitor& visitor) override { visitor.visit(*this); }

    private:
        ExprPtr expr_;
    };

    class IntLiteralExpr : public Expr
    {
    public:
        explicit IntLiteralExpr(Token int_token);

        [[nodiscard]] Token int_token() const noexcept { return int_token_; }

        void visit(ASTVisitor& visitor) override { visitor.visit(*this); }

    private:
        Token int_token_;
    };
} // namespace talos
