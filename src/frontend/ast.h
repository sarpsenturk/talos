#pragma once

#include "token.h"

#include <memory>
#include <span>
#include <vector>

namespace talos
{
    class ASTNode;
    class Expr;
    class BinaryExpr;
    class ParenExpr;
    class UnaryExpr;
    class IntLiteralExpr;
    class Statement;
    class ExprStatement;
    class FunStatement;
    class ReturnStatement;
    class VarStatement;
    class ConstStatement;
    class ProgramNode;

    using ASTNodePtr = std::unique_ptr<ASTNode>;
    using ExprPtr = std::unique_ptr<Expr>;
    using StatementPtr = std::unique_ptr<Statement>;
    using StatementList = std::vector<StatementPtr>;

    class ASTVisitor
    {
    public:
        ASTVisitor() = default;
        virtual ~ASTVisitor() = default;

        virtual void visit(const BinaryExpr& expr) = 0;
        virtual void visit(const UnaryExpr& expr) = 0;
        virtual void visit(const ParenExpr& expr) = 0;
        virtual void visit(const IntLiteralExpr& expr) = 0;
        virtual void visit(const ExprStatement& stmt) = 0;
        virtual void visit(const FunStatement& stmt) = 0;
        virtual void visit(const ReturnStatement& stmt) = 0;
        virtual void visit(const VarStatement& stmt) = 0;
        virtual void visit(const ConstStatement& stmt) = 0;
        virtual void visit(const ProgramNode& program) = 0;

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

        virtual void accept(ASTVisitor&) const = 0;

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

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        ExprPtr lhs_;
        Token op_;
        ExprPtr rhs_;
    };

    class UnaryExpr : public Expr
    {
    public:
        UnaryExpr(Token unary_op, ExprPtr expr);

        [[nodiscard]] Token unary_op() const noexcept { return unary_op_; }
        [[nodiscard]] const Expr* expr() const noexcept { return expr_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token unary_op_;
        ExprPtr expr_;
    };

    class ParenExpr : public Expr
    {
    public:
        explicit ParenExpr(ExprPtr expr);

        [[nodiscard]] const Expr* expr() const noexcept { return expr_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        ExprPtr expr_;
    };

    class IntLiteralExpr : public Expr
    {
    public:
        explicit IntLiteralExpr(Token int_token);

        [[nodiscard]] Token int_token() const noexcept { return int_token_; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token int_token_;
    };

    class Statement : public ASTNode
    {
    };

    class ExprStatement : public Statement
    {
    public:
        explicit ExprStatement(ExprPtr expr);

        [[nodiscard]] auto* expr() const noexcept { return expr_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        ExprPtr expr_;
    };

    class FunStatement : public Statement
    {
    public:
        FunStatement(Token identifier, StatementList statements);

        [[nodiscard]] auto identifier() const noexcept { return identifier_; }
        [[nodiscard]] auto statements() const noexcept { return std::span{statements_}; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token identifier_;
        StatementList statements_;
    };

    class ReturnStatement : public Statement
    {
    public:
        explicit ReturnStatement(ExprPtr return_value);

        [[nodiscard]] auto* return_value() const noexcept { return return_value_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        ExprPtr return_value_;
    };

    class VarStatement : public Statement
    {
    public:
        VarStatement(Token identifier, ExprPtr value);

        [[nodiscard]] auto identifier() const noexcept { return identifier_; }
        [[nodiscard]] auto* value() const noexcept { return value_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token identifier_;
        ExprPtr value_;
    };

    class ConstStatement : public Statement
    {
    public:
        ConstStatement(Token identifier, ExprPtr value);

        [[nodiscard]] auto identifier() const noexcept { return identifier_; }
        [[nodiscard]] auto* value() const noexcept { return value_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token identifier_;
        ExprPtr value_;
    };

    class ProgramNode : public ASTNode
    {
    public:
        explicit ProgramNode(std::vector<StatementPtr> statements);

        [[nodiscard]] auto statements() const noexcept { return std::span{statements_}; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        std::vector<StatementPtr> statements_;
    };
} // namespace talos
