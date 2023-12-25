#include "ast_printer.h"

#include <fmt/format.h>

#include <string_view>

namespace talos
{
    std::string_view type_specifier_string(const std::optional<Token>& type_spec)
    {
        if (!type_spec) {
            return "Inferred";
        }
        const auto& token = *type_spec;
        if (token.type == TokenType::Identifier) {
            return token.string;
        }
        return format_as(token.type);
    }

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

    void ASTPrinter::visit(const StringLiteralExpr& expr)
    {
        print_indented(level_, "StringLiteral {}", expr.string_literal().string);
    }

    void ASTPrinter::visit(const CharLiteralExpr& expr)
    {
        print_indented(level_, "CharacterLiteral {}", expr.char_literal().string);
    }

    void ASTPrinter::visit(const IdentifierExpr& expr)
    {
        print_indented(level_, "Identifier '{}'", expr.identifier().string);
    }

    void ASTPrinter::visit(const AssignmentExpr& expr)
    {
        print_indented(level_, "Assignment");
        ++level_;
        expr.lhs()->accept(*this);
        print_indented(level_, "operator=");
        expr.rhs()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const ExprStatement& stmt)
    {
        print_indented(level_, "ExprStatement");
        ++level_;
        stmt.expr()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const VarDeclStatement& stmt)
    {
        print_indented(level_, "VarDecl '{} {} : ({})'",
                       stmt.decl_type().string,
                       stmt.identifier().string,
                       type_specifier_string(stmt.type_specifier()));
        ++level_;
        stmt.initializer()->accept(*this);
        --level_;
    }

    void ASTPrinter::visit(const FunDeclStatement& stmt)
    {
        print_indented(level_, "FunDecl '{}() : ({})'",
                       stmt.identifier().string,
                       type_specifier_string(stmt.type_spec()));
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
