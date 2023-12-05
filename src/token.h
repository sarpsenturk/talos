#pragma once

#include "source_location.h"

namespace talos
{
    enum class TokenType {
        Eof = 0,
        Plus,
        Minus,
        Slash,
        Star,
        LeftParen,
        RightParen
    };

    struct Token {
        TokenType type;
        SourceLocation location;
    };

    constexpr auto format_as(TokenType token_type)
    {
        switch (token_type) {
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
        }
    }
}
