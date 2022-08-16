#include <cstdint>
#include <sstream>
#include <vector>
#include <array>
#include <optional>
#include <iostream>

#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vicache.h"

class IcacheModel {
 public:
    IcacheModel() {
        reset();
    }

    void reset() {
        for (auto& entry : data) {
            entry = 0;
        }
    }

    void write(uint8_t write_addr, uint8_t write_data) {
        data[write_addr] = write_data;
    }

    uint8_t read(uint8_t read_addr) {
        return data[read_addr];
    }
        
 private:
    std::array<uint8_t, 32> data;   
};

class Icache: public Testbench<Vicache> {
 protected:
    void SetUp() override {
        Testbench<Vicache>::SetUp();
    }

    void TearDown() override {
        Testbench<Vicache>::TearDown();
    }

    struct IcacheInput {
        uint8_t write;
        uint8_t write_addr;
        uint8_t write_data;
        uint8_t read_addr;
        std::optional<uint8_t> expect_read_data;
    };

    void applyInput(const IcacheInput& input) {
        dev->write = input.write;
        dev->write_addr = input.write_addr;
        dev->write_data = input.write_data;
        dev->read_addr = input.read_addr;

        dev->eval();

        // If we want to verify the outputs, do so now.
        if (input.expect_read_data) {
            EXPECT_EQ(input.expect_read_data.value(), dev->read_data) << "read_addr = " << input.read_addr;
        }

        tick();        
    }

    void applyInputs(const std::vector<IcacheInput>& inputs) {
        for (const auto& input : inputs) {
            applyInput(input);
        }
    }
};

TEST_F(Icache, resetIcache) {
    reset();

    // Read from each address
    for (uint8_t i = 0; i < 32; i++) {
        applyInput({.write = 0, .write_addr = 0x1f, .write_data = 0xff, .read_addr = i, .expect_read_data = 0x00});
    }
}

TEST_F(Icache, readAndWrite) {
    reset();

    for (uint8_t i = 1; i < 32; i++) {
        applyInput({.write = 1, .write_addr = i, .write_data = i, .read_addr = (uint8_t) (i - 1), .expect_read_data = (uint8_t) (i - 1)});
    }

    applyInput({.write = 0, .write_addr = 0x00, .write_data = 0x00, .read_addr = 31, .expect_read_data = 31});
}
