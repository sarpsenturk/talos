#include "talos.h"

#include "frontend/ast_printer.h"
#include "frontend/lexer.h"
#include "frontend/parser.h"

#include <fstream>
#include <sstream>

#include <fmt/format.h>

namespace talos
{
    VMReturn TalosVM::execute_string(std::string_view string)
    {
        auto lexer = Lexer{string};
        auto parser = Parser{&lexer};
        auto result = parser.parse();
        if (!result) {
            auto& parser_error = result.error();
            return unexpected(VMError{
                .code = parser_error.code,
                .description = fmt::format("Syntax Error ({}): {}", parser_error.location, parser_error.message),
            });
        }
        auto ast_printer = ASTPrinter{};
        ast_printer.print(*result);
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
        return execute_string(sstream.str());
    }
} // namespace talos
