#pragma once

#include "return_code.h"
#include "expected.h"

#include <string>
#include <string_view>

namespace talos {
    struct VMSuccess {
        std::string output;
    };

    struct VMError {
        ReturnCode code;
        std::string description;
    };

    using VMReturn = expected<VMSuccess, VMError>;

    class TalosVM
    {
    public:
        [[nodiscard]] VMReturn execute_string(std::string_view string);
        [[nodiscard]] VMReturn execute_file(std::string_view filename);
    };
} // namespace talos
