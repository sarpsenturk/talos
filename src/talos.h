#pragma once

#include "return_code.h"

#include <string>
#include <string_view>

namespace talos {
    struct VMReturn {
        ReturnCode code = ReturnCode::Ok;
        std::string description = return_code_desc(code);

        [[nodiscard]] bool is_ok() const noexcept { return code == ReturnCode::Ok; }
        [[nodiscard]] bool has_error() const noexcept { return code != ReturnCode::Ok; }
    };

    class TalosVM
    {
    public:
        [[nodiscard]] VMReturn execute_string(std::string_view string);
        [[nodiscard]] VMReturn execute_file(std::string_view filename);
    };
} // namespace talos
