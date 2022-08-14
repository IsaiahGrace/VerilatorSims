#include "verilated.h"
#include "gtest/gtest.h"

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
