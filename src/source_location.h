#pragma once

#include <compare>
#include <cstdint>

namespace talos {
    struct SourceLocation {
        std::int32_t line = 1;
        std::int32_t column = 1;

        void advance_line() noexcept
        {
            ++line;
            column = 1;
        }

        constexpr friend auto operator<=>(const SourceLocation& lhs, const SourceLocation& rhs) noexcept
        {
            if (lhs.line == rhs.line) {
                return lhs.column < rhs.column;
            }
            return lhs.line < rhs.line;
        }

        constexpr friend bool operator==(const SourceLocation&, const SourceLocation&) = default;
    };
}
