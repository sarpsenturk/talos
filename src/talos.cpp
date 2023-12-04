#include "talos.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace talos
{
    VMReturn TalosVM::execute_string(std::string_view string)
    {
        std::cout << string << '\n';
        return {.code = ReturnCode::Ok};
    }

    VMReturn TalosVM::execute_file(std::string_view filename)
    {
        auto source_file = std::fstream{filename};
        if (source_file.fail()) {
            return {.code = ReturnCode::FileNotFound};
        }
        return {.code = ReturnCode::Ok};
    }
} // namespace talos
