#include "ast_printer.h"

#include <fmt/format.h>

#include <string_view>

namespace talos
{
    template<typename... Args>
    void print_indented(int indent, std::string_view format_str, Args&&... args)
    {
        fmt::print("{:{}}", "", indent);
        fmt::print("{}\n", fmt::format(fmt::runtime(format_str), std::forward<Args>(args)...));
    }

    void ASTPrinter::print(const ASTNode& node)
    {
        node.accept(*this);
    }

    void ASTPrinter::visit(const BinaryExpr& expr)
    {
        print_indented(level_, "BinaryExpr");
        ++level_;
        expr.lhs()->accept(*this);
        print_indented(level_, "BinaryOp {}", expr.op().type);
        expr.rhs()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const UnaryExpr& expr)
    {
        print_indented(level_, "UnaryExpr");
        ++level_;
        print_indented(level_, "UnaryOp {}", expr.unary_op().type);
        expr.expr()->accept(*this);
    }

    void ASTPrinter::visit(const ParenExpr& expr)
    {
        print_indented(level_, "ParenExpr");
        ++level_;
        expr.expr()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const IntLiteralExpr& expr)
    {
        print_indented(level_, "IntLiteral {}", expr.int_token().string);
    }

    void ASTPrinter::visit(const ExprStatement& stmt)
    {
        print_indented(level_, "ExprStatement");
        ++level_;
        stmt.expr()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const FunStatement& stmt)
    {
        print_indented(level_, "FunStatement: {{ identifier: {} }}", stmt.identifier().string);
        ++level_;
        for (const auto& statement : stmt.statements()) {
            statement->accept(*this);
        }
        --level_;
    }

    void ASTPrinter::visit(const ReturnStatement& stmt)
    {
        print_indented(level_, "ReturnStatement");
        ++level_;
        stmt.return_value()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const ProgramNode& program)
    {
        print_indented(level_, "Program");
        ++level_;
        for (const auto& statement : program.statements()) {
            statement->accept(*this);
        }
        --level_;
    }
} // namespace talos
