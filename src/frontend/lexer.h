#pragma once

#include "expected.h"
#include "return_code.h"
#include "source_location.h"
#include "token.h"

namespace talos
{
    struct LexerError {
        ReturnCode code;
        SourceLocation location;
        std::string message;
    };

    using LexerReturn = expected<Token, LexerError>;

    class Lexer
    {
    public:
        explicit Lexer(std::string_view source);

        [[nodiscard]] LexerReturn consume_token();

    private:
        [[nodiscard]] bool is_eof() const noexcept { return current_position_ == source_.end(); }

        char consume_char() noexcept;
        char peek() const noexcept;
        void advance_line() noexcept;

        std::string_view source_;
        std::string_view::iterator current_position_;
        SourceLocation current_location_;
    };
} // namespace talos
