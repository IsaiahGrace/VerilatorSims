#include <iostream>

#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vcounter.h"

class CounterTestFixture: public ::testing::Test {
 protected:
    Testbench<Vcounter>* tb;
    void SetUp() override {
         tb = new Testbench<Vcounter>;
    }

    void TearDown() override {
        delete tb;
        tb = nullptr;
    }
};

TEST_F(CounterTestFixture, reset) {
    tb->reset();
    EXPECT_EQ(tb->dev->count, 0);
}

TEST_F(CounterTestFixture, counterOverFlow) {
    tb->reset();

    int ticks = 300;
    for (int i = 0; i < ticks; i++) {
        tb->tick();
    }

    EXPECT_EQ(tb->dev->count, ticks % (1 << 8));
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
