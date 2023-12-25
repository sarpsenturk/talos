#pragma once

#include "token.h"

#include <memory>
#include <optional>
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
    class StringLiteralExpr;
    class CharLiteralExpr;
    class IdentifierExpr;
    class AssignmentExpr;
    class Statement;
    class ExprStatement;
    class ReturnStatement;
    class VarDeclStatement;
    class FunDeclStatement;
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
        virtual void visit(const StringLiteralExpr& expr) = 0;
        virtual void visit(const CharLiteralExpr& expr) = 0;
        virtual void visit(const IdentifierExpr& expr) = 0;
        virtual void visit(const AssignmentExpr& expr) = 0;
        virtual void visit(const ExprStatement& stmt) = 0;
        virtual void visit(const ReturnStatement& stmt) = 0;
        virtual void visit(const VarDeclStatement& stmt) = 0;
        virtual void visit(const FunDeclStatement& stmt) = 0;
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

    class StringLiteralExpr : public Expr
    {
    public:
        explicit StringLiteralExpr(Token string_literal);

        [[nodiscard]] auto string_literal() const noexcept { return string_literal_; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token string_literal_;
    };

    class CharLiteralExpr : public Expr
    {
    public:
        explicit CharLiteralExpr(Token char_literal);

        [[nodiscard]] auto char_literal() const noexcept { return char_literal_; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token char_literal_;
    };

    class IdentifierExpr : public Expr
    {
    public:
        explicit IdentifierExpr(Token identifier);

        [[nodiscard]] auto identifier() const noexcept { return identifier_; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token identifier_;
    };

    class AssignmentExpr : public Expr
    {
    public:
        AssignmentExpr(ExprPtr lhs, ExprPtr rhs);

        [[nodiscard]] auto* lhs() const noexcept { return lhs_.get(); }
        [[nodiscard]] auto* rhs() const noexcept { return rhs_.get(); }

        void accept(ASTVisitor& visitor) const override { return visitor.visit(*this); }

    private:
        ExprPtr lhs_;
        ExprPtr rhs_;
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

    class ReturnStatement : public Statement
    {
    public:
        explicit ReturnStatement(ExprPtr return_value);

        [[nodiscard]] auto* return_value() const noexcept { return return_value_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        ExprPtr return_value_;
    };

    class VarDeclStatement : public Statement
    {
    public:
        VarDeclStatement(Token decl_type, Token identifier, std::optional<Token> type_spec, ExprPtr initializer);

        [[nodiscard]] auto decl_type() const noexcept { return decl_type_; }
        [[nodiscard]] auto identifier() const noexcept { return identifier_; }
        [[nodiscard]] auto type_specifier() const noexcept { return type_specifier_; }
        [[nodiscard]] auto* initializer() const noexcept { return initializer_.get(); }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token decl_type_;
        Token identifier_;
        std::optional<Token> type_specifier_;
        ExprPtr initializer_;
    };

    class FunDeclStatement : public Statement
    {
    public:
        FunDeclStatement(Token identifier, std::optional<Token> type_spec, StatementList statements);

        [[nodiscard]] auto identifier() const noexcept { return identifier_; }
        [[nodiscard]] auto type_spec() const noexcept { return type_spec_; }
        [[nodiscard]] auto statements() const noexcept { return std::span{statements_}; }

        void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }

    private:
        Token identifier_;
        std::optional<Token> type_spec_;
        StatementList statements_;
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
