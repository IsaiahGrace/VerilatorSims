#include <cstdint>
#include <sstream>
#include <vector>
#include <array>
#include <optional>
#include <iostream>

#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vtile_top.h"

class Tile_top: public Testbench<Vtile_top> {
 protected:
    void SetUp() override {
        Testbench<Vtile_top>::SetUp();
    }

    void TearDown() override {
        Testbench<Vtile_top>::TearDown();
    }

    struct meshLink {
    };

    struct Icache {
    };

    void ProgramIcache(const Icache& icache) {
    }
};

TEST_F(Tile_top, reset) {
    EXPECT_EQ(0, 0);
}
