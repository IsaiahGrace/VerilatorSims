#include <filesystem>
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    // If the "traces" directory doesn't exist, create it.
    if (!std::filesystem::is_directory("traces")) {
        std::filesystem::create_directory("traces");
    }

    // Because verilator arguments are context specific, we're not supporting arguments right now,
    // as each unit test get's it's own context.
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
