#pragma once

namespace talos {
    enum class ReturnCode {
        Ok = 0,
        FileNotFound,
    };

    [[nodiscard]] constexpr const char* return_code_desc(ReturnCode code)
    {
        switch (code) {
            case ReturnCode::Ok:
                return "No errors";
            case ReturnCode::FileNotFound:
                return "Requested file could not be found";
        }
        return "Invalid return code";
    }
}
