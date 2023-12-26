#include "exceptions.h"

namespace talos
{
    TalosException::TalosException(ReturnCode code, SourceLocation location, std::string message)
        : code_(code)
        , location_(location)
        , message_(message.empty() ? return_code_desc(code) : std::move(message))
    {
    }

    TalosException unexpected_eof(SourceLocation location, std::string message)
    {
        return {ReturnCode::UnexpectedEof, location, std::move(message)};
    }

    TalosException syntax_error(SourceLocation location, std::string message)
    {
        return {ReturnCode::SyntaxError, location, std::move(message)};
    }
    TalosException unexpected_token(SourceLocation location, std::string message)
    {
        return {ReturnCode::UnexpectedToken, location, std::move(message)};
    }
} // namespace talos
