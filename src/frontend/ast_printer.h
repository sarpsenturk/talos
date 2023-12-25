#pragma once

#include "ast.h"

namespace talos
{
    class ASTPrinter : public ASTVisitor
    {
    public:
        void print(const ASTNode& node);

    private:
        void visit(const BinaryExpr& expr) override;
        void visit(const UnaryExpr& expr) override;
        void visit(const ParenExpr& expr) override;
        void visit(const IntLiteralExpr& expr) override;
        void visit(const StringLiteralExpr& expr) override;
        void visit(const CharLiteralExpr& expr) override;
        void visit(const IdentifierExpr& expr) override;
        void visit(const AssignmentExpr& expr) override;
        void visit(const ExprStatement& stmt) override;
        void visit(const ReturnStatement& stmt) override;
        void visit(const VarDeclStatement& stmt) override;
        void visit(const FunDeclStatement& stmt) override;
        void visit(const ProgramNode& program) override;

        int level_ = 0;
    };
} // namespace talos
