#pragma once

#include <cstdint>

namespace talos {
    struct SourceLocation {
        std::int32_t line;
        std::int32_t column;
    };
}
