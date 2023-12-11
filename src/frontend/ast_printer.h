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

    public:
        void visit(const ExprStatement& stmt) override;
        void visit(const ProgramNode& program) override;

    private:
        int level_ = 0;
    };
} // namespace talos
