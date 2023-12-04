#include "talos.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace talos
{
    VMReturn TalosVM::execute_string(std::string_view string)
    {
        return VMSuccess{.output = std::string(string)};
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
