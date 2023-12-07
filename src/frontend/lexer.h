#pragma once

#include "expected.h"
#include "return_code.h"
#include "source_location.h"
#include "token.h"

#include <string_view>

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
        [[nodiscard]] bool is_eof() const noexcept { return position_ == source_.end(); }

        char consume_char() noexcept;
        void advance_line() noexcept;

        std::string_view source_;
        std::string_view::iterator position_;
        SourceLocation current_location_;
    };
} // namespace talos
