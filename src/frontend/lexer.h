#pragma once

#include "return_code.h"
#include "source_location.h"
#include "token.h"

namespace talos
{
    class Lexer
    {
    public:
        explicit Lexer(std::string_view source);

        [[nodiscard]] Token consume_token();

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
