#pragma once

#include "source_location.h"

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
        Integer,
        Semicolon,
        LeftBrace,
        RightBrace,

        // Keywords
        Fun,
        Return,
        Identifier,
        Var,
        Const
    };

    struct Token {
        TokenType type = TokenType::Invalid;
        SourceLocation location;
        std::string_view string;
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
            case TokenType::Integer:
                return "Integer";
            case TokenType::Semicolon:
                return "Semicolon";
            case TokenType::LeftBrace:
                return "Left Brace";
            case TokenType::RightBrace:
                return "Right Brace";
            case TokenType::Fun:
                return "Fun";
            case TokenType::Return:
                return "Return";
            case TokenType::Identifier:
                return "Identifier";
            case TokenType::Var:
                return "Var";
            case TokenType::Const:
                return "Const";
        }
    }
}
