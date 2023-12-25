#include "lexer.h"

#include <fmt/format.h>

#include <cctype>
#include <iterator>
#include <unordered_map>

namespace talos
{
    namespace
    {
        constexpr bool isdigit(char c)
        {
            return std::isdigit(c) != 0;
        }

        constexpr bool isalpha(char c)
        {
            return std::isalpha(c) != 0;
        }

        constexpr bool isalnum(char c)
        {
            return std::isalnum(c) != 0;
        }

        constexpr bool is_identifier_start(char c)
        {
            return isalpha(c) || c == '_';
        }

        constexpr bool is_identifier_char(char c)
        {
            return isalnum(c) || c == '_';
        }

        using namespace std::string_view_literals;
        const auto keywords = std::unordered_map{
            std::make_pair("fun"sv, TokenType::Fun),
            std::make_pair("return"sv, TokenType::Return),
            std::make_pair("var"sv, TokenType::Var),
            std::make_pair("let"sv, TokenType::Let),
            std::make_pair("i8"sv, TokenType::Int8),
            std::make_pair("i16"sv, TokenType::Int16),
            std::make_pair("i32"sv, TokenType::Int32),
            std::make_pair("i64"sv, TokenType::Int64),
            std::make_pair("f32"sv, TokenType::Float32),
            std::make_pair("f64"sv, TokenType::Float64),
        };
    } // namespace

    Lexer::Lexer(std::string_view source)
        : source_(source)
        , current_position_(source_.begin())
    {
    }

    LexerReturn Lexer::consume_token()
    {
        for (;;) {
            const auto position = current_position_;
            const auto location = current_location_;

            // Helper lambdas
            auto current_string = [&]() {
                const auto start = std::distance(source_.begin(), position);
                const auto end = std::distance(position, current_position_);
                return source_.substr(start, end);
            };
            auto make_token = [&](TokenType type) {
                return Token{
                    .type = type,
                    .location = location,
                    .string = current_string(),
                };
            };
            auto make_error = [&](ReturnCode code, std::string message) {
                return unexpected(LexerError{
                    code,
                    location,
                    std::move(message),
                });
            };
            auto invalid_char = [&](char character) {
                return make_error(ReturnCode::InvalidChar, fmt::format("Invalid character {}", character));
            };
            auto make_number = [&]() -> LexerReturn {
                auto token_type = TokenType::IntLiteral;
                while (isdigit(peek())) {
                    consume_char();
                }

                // Check for decimal exponent
                if (peek() == '.') {
                    token_type = TokenType::FloatLiteral;
                    consume_char();
                    while (isdigit(peek())) {
                        consume_char();
                    }
                }
                return make_token(token_type);
            };
            auto make_string = [&]() -> LexerReturn {
                while (peek() != '"') {
                    if (is_eof()) {
                        return make_error(ReturnCode::UnexpectedEof, "Expected terminating \"");
                    }
                    consume_char();
                }
                // Consume closing quote
                consume_char();
                return make_token(TokenType::StringLiteral);
            };
            auto make_char = [&]() -> LexerReturn {
                if (peek() == '\'') {
                    return make_error(ReturnCode::EmptyCharLiteral, "Can't have empty character literal");
                }
                if (is_eof()) {
                    return make_error(ReturnCode::UnexpectedEof, "Expected character");
                }
                consume_char();

                if (is_eof()) {
                    return make_error(ReturnCode::UnexpectedEof, "Expected terminating '");
                }
                // Consume closing quote
                consume_char();
                return make_token(TokenType::CharLiteral);
            };
            auto make_keyword_or_identifier = [&]() {
                while (is_identifier_char(peek())) {
                    consume_char();
                }

                if (auto iter = keywords.find(current_string()); iter != keywords.end()) {
                    return make_token(iter->second);
                }
                return make_token(TokenType::Identifier);
            };
            //

            const auto character = consume_char();
            switch (character) {
                case '\0':
                    return make_token(TokenType::Eof);
                case '+':
                    return make_token(TokenType::Plus);
                case '-':
                    return make_token(TokenType::Minus);
                case '/':
                    return make_token(TokenType::Slash);
                case '*':
                    return make_token(TokenType::Star);
                case '(':
                    return make_token(TokenType::LeftParen);
                case ')':
                    return make_token(TokenType::RightParen);
                case ';':
                    return make_token(TokenType::Semicolon);
                case '{':
                    return make_token(TokenType::LeftBrace);
                case '}':
                    return make_token(TokenType::RightBrace);
                case '=':
                    return make_token(TokenType::Equal);
                case ':':
                    return make_token(TokenType::Colon);
                case ' ':
                    continue;
                case '\t':
                    current_location_.column += 3;
                    continue;
                case '\n':
                    advance_line();
                    continue;
                case '"':
                    return make_string();
                case '\'':
                    return make_char();
                default:
                    if (isdigit(character)) {
                        return make_number();
                    }
                    if (is_identifier_start(character)) {
                        return make_keyword_or_identifier();
                    }
                    break;
            }
            return invalid_char(character);
        }
    }

    char Lexer::consume_char() noexcept
    {
        if (is_eof()) {
            return '\0';
        }
        ++current_location_.column;
        return *(current_position_++);
    }

    char Lexer::peek() const noexcept
    {
        if (is_eof()) {
            return '\0';
        }
        return *current_position_;
    }

    void Lexer::advance_line() noexcept
    {
        current_location_.advance_line();
    }
} // namespace talos
