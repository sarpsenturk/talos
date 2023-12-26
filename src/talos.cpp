#include "talos.h"

#include "exceptions.h"
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
        try {
            auto lexer = Lexer{string};
            auto parser = Parser{&lexer};
            auto result = parser.parse();
            auto ast_printer = ASTPrinter{};
            ast_printer.print(result);
            return VMSuccess{.output = ""};
        } catch (const TalosException& exception) {
            return unexpected(VMError{
                .code = exception.code(),
                .description = fmt::format("{} ({}): {}",
                                           return_code_str(exception.code()),
                                           exception.location(),
                                           exception.what()),
            });
        }
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
