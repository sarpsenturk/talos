#pragma once

#include "return_code.h"
#include "source_location.h"

#include <exception>
#include <string>

namespace talos
{
    class TalosException : public std::exception
    {
    public:
        TalosException(ReturnCode code, SourceLocation location, std::string message = "");

        [[nodiscard]] ReturnCode code() const noexcept { return code_; }
        [[nodiscard]] SourceLocation location() const noexcept { return location_; }
        [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); }

    private:
        ReturnCode code_;
        SourceLocation location_;
        std::string message_;
    };

    TalosException unexpected_eof(SourceLocation location, std::string message = "");
    TalosException syntax_error(SourceLocation location, std::string message = "");
    TalosException unexpected_token(SourceLocation location, std::string message = "");
} // namespace talos
