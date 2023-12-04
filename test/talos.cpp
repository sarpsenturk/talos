#include "talos.h"

#include <gtest/gtest.h>

namespace
{
    TEST(Talos, Talos)
    {
        EXPECT_EQ(talos::return_ok(), 0);
    }
} // namespace
