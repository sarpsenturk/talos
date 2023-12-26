#pragma once

#include "source_location.h"

#include <array>
#include <string>
#include <string_view>

namespace talos
{
    enum class TokenType {
        Invalid = -1,
        Eof = 0,
        Plus,
        Minus,
        Slash,
        Star,
        LeftParen,
        RightParen,
        Semicolon,
        LeftBrace,
        RightBrace,
        Equal,
        Colon,

        // Keywords
        Fun,
        Return,
        Identifier,
        Var,
        Let,

        // Builtin type keywords
        Int8,
        Int16,
        Int32,
        Int64,
        Float32,
        Float64,
        Bool,

        // Literal tokens
        IntLiteral,
        FloatLiteral,
        StringLiteral,
        CharLiteral,
        TrueLiteral,
        FalseLiteral,
    };

    inline constexpr auto type_keywords = std::array{
        TokenType::Int8,
        TokenType::Int16,
        TokenType::Int32,
        TokenType::Int64,
        TokenType::Float32,
        TokenType::Float64,
        TokenType::Bool,
    };

    struct Token {
        TokenType type = TokenType::Invalid;
        SourceLocation location;
        std::string_view string;
    };

    template<typename F>
    concept TokenPredicate = requires(F callable, const Token& token) {
        {
            callable(token)
        } -> std::convertible_to<bool>;
    };

    constexpr auto format_as(TokenType token_type)
    {
        switch (token_type) {
            case TokenType::Invalid:
                return "Invalid";
            case TokenType::Eof:
                return "Eof";
            case TokenType::Plus:
                return "Plus";
            case TokenType::Minus:
                return "Minus";
            case TokenType::Slash:
                return "Slash";
            case TokenType::Star:
                return "Star";
            case TokenType::LeftParen:
                return "Left Paren";
            case TokenType::RightParen:
                return "Right Paren";
            case TokenType::Semicolon:
                return "Semicolon";
            case TokenType::LeftBrace:
                return "Left Brace";
            case TokenType::RightBrace:
                return "Right Brace";
            case TokenType::Equal:
                return "Equal";
            case TokenType::Colon:
                return "Colon";
            case TokenType::Fun:
                return "Fun";
            case TokenType::Return:
                return "Return";
            case TokenType::Identifier:
                return "Identifier";
            case TokenType::Var:
                return "Var";
            case TokenType::Let:
                return "Let";
            case TokenType::Int8:
                return "Int8";
            case TokenType::Int16:
                return "Int16";
            case TokenType::Int32:
                return "Int32";
            case TokenType::Int64:
                return "Int64";
            case TokenType::Float32:
                return "Float32";
            case TokenType::Float64:
                return "Float64";
            case TokenType::IntLiteral:
                return "IntLiteral Literal";
            case TokenType::FloatLiteral:
                return "FloatLiteral Literal";
            case TokenType::StringLiteral:
                return "StringLiteral Literal";
            case TokenType::CharLiteral:
                return "CharLiteral Literal";
            case TokenType::TrueLiteral:
                return "True";
            case TokenType::FalseLiteral:
                return "False";
            case TokenType::Bool:
                return "Bool";
        }
    }
} // namespace talos
