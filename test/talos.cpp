#include "talos.h"

#include <gtest/gtest.h>

namespace
{
    TEST(TalosVM, File)
    {
        // Invalid file
        {
            auto vm = talos::TalosVM{};
            const auto result = vm.execute_file("invalid_file_name");
            EXPECT_FALSE(result);
            const auto& error = result.error();
            EXPECT_EQ(error.code, talos::ReturnCode::FileNotFound);
        }
    }
} // namespace
