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

    public:
        void visit(const UnaryExpr& expr) override;

    private:
        void visit(const ParenExpr& expr) override;
        void visit(const IntLiteralExpr& expr) override;

        int level_ = 0;
    };
} // namespace talos
