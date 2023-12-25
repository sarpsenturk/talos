#pragma once

namespace talos {
    enum class ReturnCode {
        Ok = 0,
        FileNotFound,
        InvalidChar,
        SyntaxError,
        UnexpectedToken,
        UnexpectedEof,
        UnexpectedChar,
        EmptyCharLiteral
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
            case ReturnCode::UnexpectedEof:
                return "Unexpected end of file";
            case ReturnCode::UnexpectedChar:
                return "Unexpected character";
            case ReturnCode::EmptyCharLiteral:
                return "Empty character literal";
        }
        return "Invalid return code";
    }
}
