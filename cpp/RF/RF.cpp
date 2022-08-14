#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "VRF.h"

class RF: public ::testing::Test {
 protected:
    Testbench<VRF>* tb;
    void SetUp() override {
         tb = new Testbench<VRF>;
    }

    void TearDown() override {
        delete tb;
        tb = nullptr;
    }
};

TEST_F(RF, resetACC) {
    tb->reset();
    EXPECT_EQ(tb->dev->acc, 0);
}

TEST_F(RF, resetBAK) {
    tb->reset();
    tb->dev->swap = 1;
    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0);
}

TEST_F(RF, writeACC) {
    tb->reset();

    tb->dev->write = 1;
    tb->dev->write_data = 0xaa;

    tb->tick();

    EXPECT_EQ(tb->dev->acc, 0xaa);
}

TEST_F(RF, swap) {
    tb->reset();

    tb->dev->write = 1;
    tb->dev->write_data = 0xaa;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0xaa);

    tb->dev->write = 0;
    tb->dev->swap = 1;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0x00);

    // swap is still active!

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0xaa);
}

TEST_F(RF, save) {
    tb->reset();
    EXPECT_EQ(tb->dev->acc, 0x00);

    // Write 0xaa to ACC
    tb->dev->write = 1;
    tb->dev->write_data = 0xaa;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0xaa);

    // Swap ACC and BAK
    tb->dev->write = 0;
    tb->dev->swap = 1;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0x00);

    // Write 0xbb to ACC
    tb->dev->swap = 0;
    tb->dev->write = 1;
    tb->dev->write_data = 0xbb;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0xbb);

    // Save ACC to BAK
    tb->dev->write = 0;
    tb->dev->save = 1;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0xbb);

    // SWAP ACC and BAK (so we can verify save)
    tb->dev->save = 0;
    tb->dev->swap = 1;

    tb->tick();
    EXPECT_EQ(tb->dev->acc, 0xbb);
}
