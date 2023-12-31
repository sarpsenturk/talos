#include "talos.h"

#include <iostream>
#include <string>

int run_file(talos::TalosVM& vm, const char* filename)
{
    const auto result = vm.execute_file(filename);
    if (!result) {
        const auto& error = result.error();
        std::cerr << error.description << '\n';
        return static_cast<int>(error.code);
    }
    return 0;
}

int run_repl(talos::TalosVM& vm)
{
    std::string input;
    for (;;) {
        std::cout << "> ";
        if (!std::getline(std::cin, input) || input == "exit") {
            break;
        }
        const auto result = vm.execute_string(input);
        if (!result) {
            const auto& error = result.error();
            std::cerr << error.description << '\n';
            return static_cast<int>(error.code);
        }
        std::cout << result->output << '\n';
    }
    return 0;
}

int main(int argc, const char* argv[])
{
    auto talos_vm = talos::TalosVM{};
    if (argc == 1) {
        return run_repl(talos_vm);
    }
    else if (argc == 2) {
        return run_file(talos_vm, argv[1]);
    }
    std::cerr << "Invalid arguments. Usage:\ntalos [filename]\n";
    return -1;
}
