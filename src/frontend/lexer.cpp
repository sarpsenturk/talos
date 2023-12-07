#include "lexer.h"

#include <fmt/format.h>

namespace talos
{
    Lexer::Lexer(std::string_view source)
        : source_(source)
        , position_(source_.begin())
    {
    }

    LexerReturn Lexer::consume_token()
    {
        for (;;) {
            const auto location = current_location_;
            auto make_token = [&](TokenType type) { return Token{.type = type, .location = location}; };
            auto make_error = [&](ReturnCode code, std::string message) { return unexpected(LexerError{code, location, std::move(message)}); };
            auto invalid_char = [&](char character) { return make_error(ReturnCode::InvalidChar, fmt::format("Invalid character {}", character)); };

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
                case ' ':
                    continue;
                case '\t':
                    current_location_.column += 3;
                    continue;
                case '\n':
                    advance_line();
                    continue;
                default:
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
        return *(position_++);
    }

    void Lexer::advance_line() noexcept
    {
        current_location_.advance_line();
    }
} // namespace talos
