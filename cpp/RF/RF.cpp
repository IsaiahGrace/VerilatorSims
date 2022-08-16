#include <cstdint>
#include <sstream>
#include <vector>

#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "VRF.h"

class RF: public ::testing::Test {
 protected:
    Testbench<VRF>* tb;

    void SetUp() override {
        const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
        std::ostringstream testName;
        testName << test_info->test_suite_name() << "-" << test_info->name();
        tb = new Testbench<VRF>(testName.str());
    }

    void TearDown() override {
        // One final tick, just to make the waveform traces readable.
        tb->tick();

        delete tb;
        tb = nullptr;
    }

    struct RFInput {
        uint8_t write;
        uint8_t swap;
        uint8_t save;
        uint8_t write_data;
        uint8_t expect_acc;
    };

    void applyInput(const RFInput& inputs) {
        tb->dev->write = inputs.write;
        tb->dev->swap = inputs.swap;
        tb->dev->save = inputs.save;
        tb->dev->write_data = inputs.write_data;
        tb->tick();
        EXPECT_EQ(inputs.expect_acc, tb->dev->acc);
    }
};

TEST_F(RF, resetACC) {
    tb->reset();
    EXPECT_EQ(tb->dev->acc, 0);
}

TEST_F(RF, resetBAK) {
    tb->reset();

    applyInput({
        .write=0,
        .swap=1,
        .save=0,
        .write_data=0xff,
        .expect_acc=0x00
    });
}

TEST_F(RF, writeACC) {
    tb->reset();

    applyInput({
        .write=1,
        .swap=0,
        .save=0,
        .write_data=0xaa,
        .expect_acc=0xaa
    });
}

TEST_F(RF, swap) {
    tb->reset();

    applyInput({
        .write=1,
        .swap=0,
        .save=0,
        .write_data=0xaa,
        .expect_acc=0xaa
    });

    applyInput({
        .write=0,
        .swap=1,
        .save=0,
        .write_data=0xff,
        .expect_acc=0x00
    });

    applyInput({
        .write=0,
        .swap=1,
        .save=0,
        .write_data=0xff,
        .expect_acc=0xaa
    });
}

TEST_F(RF, save) {
    tb->reset();

    std::vector<RFInput> inputs = {
        {.write=1, .swap=0, .save=0, .write_data=0xaa, .expect_acc=0xaa},  // Write 0xaa to ACC
        {.write=0, .swap=1, .save=0, .write_data=0xff, .expect_acc=0x00},  // Swap ACC and BAK
        {.write=1, .swap=0, .save=0, .write_data=0xbb, .expect_acc=0xbb},  // Write 0xbb to ACC
        {.write=0, .swap=0, .save=1, .write_data=0xff, .expect_acc=0xbb},  // Save ACC to BAK
        {.write=1, .swap=0, .save=0, .write_data=0xaa, .expect_acc=0xaa},  // Write 0xaa to ACC
        {.write=0, .swap=1, .save=0, .write_data=0xff, .expect_acc=0xbb},  // SWAP ACC and BAK (so we can verify save)
        {.write=0, .swap=1, .save=0, .write_data=0xff, .expect_acc=0xaa},  // SWAP ACC and BAK (again! for fun.)
    };

    for (const auto& input : inputs) {
        applyInput(input);
    }
}
