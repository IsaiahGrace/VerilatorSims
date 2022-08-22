#include <cstdint>
#include <sstream>
#include <vector>

#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vregister_file.h"

class register_file: public Testbench<Vregister_file> {
 protected:
    void SetUp() override {
        Testbench<Vregister_file>::SetUp();
    }

    void TearDown() override {
        Testbench<Vregister_file>::TearDown();
    }

    struct RFInput {
        uint8_t write;
        uint8_t swap;
        uint8_t save;
        uint8_t write_data;
        uint8_t expect_acc;
    };

    void applyInput(const RFInput& input) {
        dev->write = input.write;
        dev->swap = input.swap;
        dev->save = input.save;
        dev->write_data = input.write_data;
        tick();
        EXPECT_EQ(input.expect_acc, dev->acc);
    }

    void applyInputs(const std::vector<RFInput>& inputs) {
        for (const auto& input : inputs) {
            applyInput(input);
        }
    }
};

TEST_F(register_file, resetACC) {
    reset();
    EXPECT_EQ(dev->acc, 0);
}

TEST_F(register_file, resetBAK) {
    reset();

    applyInputs({
        {.write = 0, .swap = 1, .save = 0, .write_data = 0xff, .expect_acc = 0x00}
    });
}

TEST_F(register_file, writeACC) {
    reset();

    applyInputs({
        {.write = 1, .swap = 0, .save = 0, .write_data = 0xaa, .expect_acc = 0xaa}
    });
}

TEST_F(register_file, swap) {
    reset();

    applyInputs({
        {.write = 1, .swap = 0, .save = 0, .write_data = 0xaa, .expect_acc = 0xaa},
        {.write = 0, .swap = 1, .save = 0, .write_data = 0xff, .expect_acc = 0x00},
        {.write = 0, .swap = 1, .save = 0, .write_data = 0xff, .expect_acc = 0xaa},
    });
}

TEST_F(register_file, save) {
    reset();

    applyInputs({
        {.write = 1, .swap = 0, .save = 0, .write_data = 0xaa, .expect_acc = 0xaa},  // Write 0xaa to ACC
        {.write = 0, .swap = 1, .save = 0, .write_data = 0xff, .expect_acc = 0x00},  // Swap ACC and BAK
        {.write = 1, .swap = 0, .save = 0, .write_data = 0xbb, .expect_acc = 0xbb},  // Write 0xbb to ACC
        {.write = 0, .swap = 0, .save = 1, .write_data = 0xff, .expect_acc = 0xbb},  // Save ACC to BAK
        {.write = 1, .swap = 0, .save = 0, .write_data = 0xaa, .expect_acc = 0xaa},  // Write 0xaa to ACC
        {.write = 0, .swap = 1, .save = 0, .write_data = 0xff, .expect_acc = 0xbb},  // SWAP ACC and BAK (so we can verify save)
        {.write = 0, .swap = 1, .save = 0, .write_data = 0xff, .expect_acc = 0xaa},  // SWAP ACC and BAK (again! for fun.)
    });
}
