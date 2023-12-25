#include "frontend/lexer.h"

#include <gtest/gtest.h>

namespace
{
    void expect_token_type(const talos::LexerReturn& result, talos::TokenType type)
    {
        ASSERT_TRUE(result);
        EXPECT_EQ(result->type, type);
    }

    void expect_token_location(const talos::LexerReturn& result, const talos::SourceLocation& location)
    {
        ASSERT_TRUE(result);
        EXPECT_EQ(result->location, location);
    }

    void expect_token_string(const talos::LexerReturn& result, std::string_view string)
    {
        ASSERT_TRUE(result);
        EXPECT_EQ(result->string, string);
    }

    void expect_lexer_error(const talos::LexerReturn& result, talos::ReturnCode code)
    {
        ASSERT_FALSE(result);
        EXPECT_EQ(result.error().code, code);
    }

    TEST(Lexer, Tokens)
    {
        constexpr const char* string = "+-/*();{}=:";
        auto lexer = talos::Lexer{string};
        using enum talos::TokenType;
        expect_token_type(lexer.consume_token(), Plus);
        expect_token_type(lexer.consume_token(), Minus);
        expect_token_type(lexer.consume_token(), Slash);
        expect_token_type(lexer.consume_token(), Star);
        expect_token_type(lexer.consume_token(), LeftParen);
        expect_token_type(lexer.consume_token(), RightParen);
        expect_token_type(lexer.consume_token(), Semicolon);
        expect_token_type(lexer.consume_token(), LeftBrace);
        expect_token_type(lexer.consume_token(), RightBrace);
        expect_token_type(lexer.consume_token(), Equal);
        expect_token_type(lexer.consume_token(), Colon);
        expect_token_type(lexer.consume_token(), Eof);
    }

    TEST(Lexer, Location)
    {
        // Whitespace
        {
            constexpr const char* string = "\t+ +";
            auto lexer = talos::Lexer{string};
            expect_token_location(lexer.consume_token(), {.line = 1, .column = 5});
            expect_token_location(lexer.consume_token(), {.line = 1, .column = 7});
        }

        // New line
        {
            constexpr const char* string = "\n+\n\n+";
            auto lexer = talos::Lexer{string};
            expect_token_location(lexer.consume_token(), {.line = 2, .column = 1});
            expect_token_location(lexer.consume_token(), {.line = 4, .column = 1});
        }
    }

    TEST(Lexer, Empty)
    {
        auto lexer = talos::Lexer{""};
        const auto result = lexer.consume_token();
        expect_token_type(result, talos::TokenType::Eof);
        expect_token_location(result, {.line = 1, .column = 1});
    }

    TEST(Lexer, IntLiteral)
    {
        constexpr const char* string = "1234567890";
        auto lexer = talos::Lexer{string};
        const auto result = lexer.consume_token();
        expect_token_type(result, talos::TokenType::IntLiteral);
        expect_token_string(result, "1234567890");
    }

    TEST(Lexer, StringLiterals)
    {
        // Basic string literal
        {
            constexpr auto string = "\"string\"";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            expect_token_type(result, talos::TokenType::StringLiteral);
            expect_token_string(result, "\"string\"");
        }

        // Empty string literal
        {
            constexpr auto string = "\"\"";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            expect_token_type(result, talos::TokenType::StringLiteral);
            expect_token_string(result, "\"\"");
        }

        // Missing terminator
        {
            constexpr auto string = "\"string";
            auto lexer = talos::Lexer{string};
            expect_lexer_error(lexer.consume_token(), talos::ReturnCode::UnexpectedEof);
        }
    }

    TEST(Lexer, CharLiterals)
    {
        // Basic character literal
        {
            constexpr auto string = "'c'";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            expect_token_type(result, talos::TokenType::CharLiteral);
            expect_token_string(result, "'c'");
        }

        // Empty character literal
        {
            constexpr auto string = "''";
            auto lexer = talos::Lexer{string};
            expect_lexer_error(lexer.consume_token(), talos::ReturnCode::EmptyCharLiteral);
        }

        // Missing character
        {
            constexpr auto string = "'";
            auto lexer = talos::Lexer{string};
            expect_lexer_error(lexer.consume_token(), talos::ReturnCode::UnexpectedEof);
        }

        // Missing terminator
        {
            constexpr auto string = "'c";
            auto lexer = talos::Lexer{string};
            expect_lexer_error(lexer.consume_token(), talos::ReturnCode::UnexpectedEof);
        }
    }

    TEST(Lexer, FloatLiterals)
    {
        // Basic float literal
        {
            constexpr auto string = "1.0";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            expect_token_type(result, talos::TokenType::FloatLiteral);
            expect_token_string(result, "1.0");
        }

        // Optional decimal exponent
        {
            constexpr auto string = "1.";
            auto lexer = talos::Lexer{string};
            const auto result = lexer.consume_token();
            expect_token_type(result, talos::TokenType::FloatLiteral);
            expect_token_string(result, "1.");
        }
    }

    TEST(Lexer, Keywords)
    {
        constexpr const char* string = "fun return var let";
        auto lexer = talos::Lexer{string};
        using enum talos::TokenType;
        expect_token_type(lexer.consume_token(), Fun);
        expect_token_type(lexer.consume_token(), Return);
        expect_token_type(lexer.consume_token(), Var);
        expect_token_type(lexer.consume_token(), Let);
        expect_token_type(lexer.consume_token(), Eof);
    }

    TEST(Lexer, BuiltinTypes)
    {
        constexpr const char* string = "i8 i16 i32 i64 f32 f64";
        auto lexer = talos::Lexer{string};
        using enum talos::TokenType;
        expect_token_type(lexer.consume_token(), Int8);
        expect_token_type(lexer.consume_token(), Int16);
        expect_token_type(lexer.consume_token(), Int32);
        expect_token_type(lexer.consume_token(), Int64);
        expect_token_type(lexer.consume_token(), Float32);
        expect_token_type(lexer.consume_token(), Float64);
    }
} // namespace
