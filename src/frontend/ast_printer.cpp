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
} // namespace talos
