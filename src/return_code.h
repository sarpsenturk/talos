#pragma once

namespace talos {
    enum class ReturnCode {
        Ok = 0,
        FileNotFound,
        InvalidChar,
        SyntaxError,
        UnexpectedToken
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
            case ReturnCode::SyntaxError:
                return "Syntax Error";
            case ReturnCode::UnexpectedToken:
                return "Unexpected token";
        }
        return "Invalid return code";
    }
}
