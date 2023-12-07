#pragma once

namespace talos {
    enum class ReturnCode {
        Ok = 0,
        FileNotFound,
        InvalidChar
    };

    [[nodiscard]] constexpr const char* return_code_desc(ReturnCode code)
    {
        switch (code) {
            case ReturnCode::Ok:
                return "No errors";
            case ReturnCode::FileNotFound:
                return "Requested file could not be found";
            case ReturnCode::InvalidChar:
                return "Invalid character";
        }
        return "Invalid return code";
    }
}
