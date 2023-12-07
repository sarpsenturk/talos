#include "talos.h"

#include "frontend/lexer.h"

#include <spdlog/spdlog.h>

#include <fstream>
#include <sstream>

namespace talos
{
    VMReturn TalosVM::execute_string(std::string_view string)
    {
        auto lexer = Lexer{string};
        for (;;) {
            const auto result = lexer.consume_token();
            if (!result) {
                const auto& error = result.error();
                SPDLOG_ERROR("Lexer error ({}:{}): {}",
                             error.location.line, error.location.column,
                             error.message);
                return unexpected(VMError{error.code});
            }
            const auto& token = *result;
            SPDLOG_INFO("Token{{ type: {}, location: {}:{} }}",
                        token.type,
                        token.location.line, token.location.column);
            if (token.type == TokenType::Eof) {
                break;
            }
        }
        return VMSuccess{.output = ""};
    }

    VMReturn TalosVM::execute_file(std::string_view filename)
    {
        auto source_file = std::fstream{filename};
        if (source_file.fail()) {
            return unexpected(VMError{.code = ReturnCode::FileNotFound});
        }
        std::stringstream sstream;
        sstream << source_file.rdbuf();
        return VMSuccess{.output = sstream.str()};
    }
} // namespace talos
