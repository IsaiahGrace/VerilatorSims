#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vcounter.h"

class Counter: public Testbench<Vcounter> {
 protected:
    void SetUp() override {
        Testbench<Vcounter>::SetUp();
    }

    void TearDown() override {
        Testbench<Vcounter>::TearDown();
    }
};

TEST_F(Counter, reset) {
    reset();
    EXPECT_EQ(dev->count, 0);
}

TEST_F(Counter, countThenReset) {
    reset();
    tick();
    tick();
    EXPECT_EQ(dev->count, 2);
    reset();
    EXPECT_EQ(dev->count, 0);
}

TEST_F(Counter, counterOverFlow) {
    reset();

    int ticks = 300;
    for (int i = 0; i < ticks; i++) {
        EXPECT_EQ(dev->count, i % (1 << 8));
        tick();
    }
    EXPECT_EQ(dev->count, ticks % (1 << 8));
}
