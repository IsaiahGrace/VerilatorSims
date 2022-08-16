#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vcounter.h"

class Counter: public ::testing::Test {
 protected:
    Testbench<Vcounter>* tb;

    void SetUp() override {
        const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
        std::ostringstream testName;
        testName << "traces/" << test_info->test_suite_name() << "-" << test_info->name() << ".vcd";
        tb = new Testbench<Vcounter>(testName.str());
    }

    void TearDown() override {
        delete tb;
        tb = nullptr;
    }
};

TEST_F(Counter, reset) {
    tb->reset();
    EXPECT_EQ(tb->dev->count, 0);
}

TEST_F(Counter, countThenReset) {
    tb->reset();
    tb->tick();
    tb->tick();
    EXPECT_EQ(tb->dev->count, 2);
    tb->reset();
    EXPECT_EQ(tb->dev->count, 0);
}

TEST_F(Counter, counterOverFlow) {
    tb->reset();

    int ticks = 300;
    for (int i = 0; i < ticks; i++) {
        EXPECT_EQ(tb->dev->count, i % (1 << 8));
        tb->tick();
    }
    EXPECT_EQ(tb->dev->count, ticks % (1 << 8));
}
