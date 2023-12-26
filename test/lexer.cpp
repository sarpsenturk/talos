#include "frontend/lexer.h"
#include "exceptions.h"

#include <gtest/gtest.h>

#define EXPECT_TOKEN_TYPE(result, expected) \
    EXPECT_EQ((result).type, (expected))

#define EXPECT_TOKEN_LOCATION(result, line, column) \
    EXPECT_EQ((result).location, (talos::SourceLocation{line, column}))

#define EXPECT_TOKEN_STRING(result, expected) \
    EXPECT_EQ((result).string, (expected))

#define EXPECT_LEXER_ERROR(expression, expected)   \
    {                                              \
        bool exception_caught = false;             \
        try {                                      \
            (void)(expression);                    \
        } catch (const talos::TalosException& e) { \
            exception_caught = true;               \
            EXPECT_EQ(e.code(), expected);         \
        }                                          \
    }

namespace
{
    TEST(Lexer, Tokens)
    {
        constexpr const char* string = "+-/*();{}=:";
        auto lexer = talos::Lexer{string};
        using enum talos::TokenType;
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Plus);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Minus);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Slash);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Star);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), LeftParen);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), RightParen);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Semicolon);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), LeftBrace);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), RightBrace);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Equal);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Colon);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Eof);
    }

    TEST(Lexer, Location)
    {
        // Whitespace
        {
            constexpr const char* string = "\t+ +";
            auto lexer = talos::Lexer{string};
            EXPECT_TOKEN_LOCATION(lexer.consume_token(), 1, 5);
            EXPECT_TOKEN_LOCATION(lexer.consume_token(), 1, 7);
        }

        // New line
        {
            constexpr const char* string = "\n+\n\n+";
            auto lexer = talos::Lexer{string};
            EXPECT_TOKEN_LOCATION(lexer.consume_token(), 2, 1);
            EXPECT_TOKEN_LOCATION(lexer.consume_token(), 4, 1);
        }
    }

    TEST(Lexer, Empty)
    {
        auto lexer = talos::Lexer{""};
        const auto result = lexer.consume_token();
        EXPECT_TOKEN_TYPE(result, talos::TokenType::Eof);
        EXPECT_TOKEN_LOCATION(result, 1, 1);
    }

    TEST(Lexer, IntLiteral)
    {
        constexpr const char* string = "1234567890";
        auto lexer = talos::Lexer{string};
        const auto result = lexer.consume_token();
        EXPECT_TOKEN_TYPE(result, talos::TokenType::IntLiteral);
        EXPECT_TOKEN_STRING(result, "1234567890");
    }

    TEST(Lexer, StringLiterals)
    {
        // Basic string literal
        {
            constexpr auto string = "\"string\"";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            EXPECT_TOKEN_TYPE(result, talos::TokenType::StringLiteral);
            EXPECT_TOKEN_STRING(result, "\"string\"");
        }

        // Empty string literal
        {
            constexpr auto string = "\"\"";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            EXPECT_TOKEN_TYPE(result, talos::TokenType::StringLiteral);
            EXPECT_TOKEN_STRING(result, "\"\"");
        }

        // Missing terminator
        {
            constexpr auto string = "\"string";
            auto lexer = talos::Lexer{string};
            EXPECT_LEXER_ERROR(lexer.consume_token(), talos::ReturnCode::UnexpectedEof);
        }
    }

    TEST(Lexer, CharLiterals)
    {
        // Basic character literal
        {
            constexpr auto string = "'c'";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            EXPECT_TOKEN_TYPE(result, talos::TokenType::CharLiteral);
            EXPECT_TOKEN_STRING(result, "'c'");
        }

        // Empty character literal
        {
            constexpr auto string = "''";
            auto lexer = talos::Lexer{string};
            EXPECT_LEXER_ERROR(lexer.consume_token(), talos::ReturnCode::EmptyCharLiteral);
        }

        // Missing character
        {
            constexpr auto string = "'";
            auto lexer = talos::Lexer{string};
            EXPECT_LEXER_ERROR(lexer.consume_token(), talos::ReturnCode::UnexpectedEof);
        }

        // Missing terminator
        {
            constexpr auto string = "'c";
            auto lexer = talos::Lexer{string};
            EXPECT_LEXER_ERROR(lexer.consume_token(), talos::ReturnCode::UnexpectedEof);
        }
    }

    TEST(Lexer, FloatLiterals)
    {
        // Basic float literal
        {
            constexpr auto string = "1.0";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            EXPECT_TOKEN_TYPE(result, talos::TokenType::FloatLiteral);
            EXPECT_TOKEN_STRING(result, "1.0");
        }

        // Optional decimal exponent
        {
            constexpr auto string = "1.";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            EXPECT_TOKEN_TYPE(result, talos::TokenType::FloatLiteral);
            EXPECT_TOKEN_STRING(result, "1.");
        }
    }

    TEST(Lexer, Keywords)
    {
        constexpr const char* string = "fun return var let true false";
        auto lexer = talos::Lexer{string};
        using enum talos::TokenType;
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Fun);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Return);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Var);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Let);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), TrueLiteral);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), FalseLiteral);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Eof);
    }

    TEST(Lexer, BuiltinTypes)
    {
        constexpr const char* string = "i8 i16 i32 i64 f32 f64 bool";
        auto lexer = talos::Lexer{string};
        using enum talos::TokenType;
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Int8);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Int16);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Int32);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Int64);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Float32);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Float64);
        EXPECT_TOKEN_TYPE(lexer.consume_token(), Bool);
    }
} // namespace
