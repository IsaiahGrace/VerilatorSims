#include <cstdint>
#include <sstream>
#include <vector>
#include <array>
#include <optional>
#include <iostream>

#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"
#include "types.hpp"

#include "VtileIO_top.h"

class TileIO: public Testbench<VtileIO_top> {
 protected:
    void SetUp() override {
        Testbench<VtileIO_top>::SetUp();
    }

    void TearDown() override {
        Testbench<VtileIO_top>::TearDown();
    }

    struct TileIF {
        // Module inputs:
        Word  in_recv_data;
        Logic in_recv_valid;
        Logic in_send_done;

        // Module outputs:
        std::optional<Word>  out_send_data;
        std::optional<Logic> out_send_ready;
        std::optional<Logic> out_recv_ready;
    };

    struct TileIOInput {
        Location in_loc;

        Word in_send_data;
        Logic in_send;
        std::optional<Logic> out_send_done;

        std::optional<Word> out_recv_data;
        Logic in_recv;
        std::optional<Logic> out_recv_valid;

        TileIF up, down, left, right;
    };


    void applyInput(const TileIOInput& input) {
        // Drive module inputs
        dev->loc = input.in_loc;
        dev->send = input.in_send;
        dev->recv = input.in_recv;
        dev->send_data = input.in_send_data;

        dev->up_send_done  = input.up.in_send_done;
        dev->up_recv_data  = input.up.in_recv_data;
        dev->up_recv_valid = input.up.in_recv_valid;

        dev->down_send_done  = input.down.in_send_done;
        dev->down_recv_data  = input.down.in_recv_data;
        dev->down_recv_valid = input.down.in_recv_valid;

        dev->left_send_done  = input.left.in_send_done;
        dev->left_recv_data  = input.left.in_recv_data;
        dev->left_recv_valid = input.left.in_recv_valid;

        dev->right_send_done  = input.right.in_send_done;
        dev->right_recv_data  = input.right.in_recv_data;
        dev->right_recv_valid = input.right.in_recv_valid;

        // Simulate a clock cycle
        tick();

        // Verify module outputs (only if expectations exist)
        if (input.out_send_done)  {EXPECT_EQ(input.out_send_done, dev->send_done);}
        if (input.out_recv_data)  {EXPECT_EQ(input.out_recv_data, dev->recv_data);}
        if (input.out_recv_valid) {EXPECT_EQ(input.out_recv_valid, dev->recv_valid);}

        if (input.up.out_send_data)  {EXPECT_EQ(input.up.out_send_data,  dev->up_send_data);}
        if (input.up.out_send_ready) {EXPECT_EQ(input.up.out_send_ready, dev->up_send_ready);}
        if (input.up.out_recv_ready) {EXPECT_EQ(input.up.out_recv_ready, dev->up_recv_ready);}

        if (input.down.out_send_data)  {EXPECT_EQ(input.down.out_send_data,  dev->down_send_data);}
        if (input.down.out_send_ready) {EXPECT_EQ(input.down.out_send_ready, dev->down_send_ready);}
        if (input.down.out_recv_ready) {EXPECT_EQ(input.down.out_recv_ready, dev->down_recv_ready);}

        if (input.left.out_send_data)  {EXPECT_EQ(input.left.out_send_data,  dev->left_send_data);}
        if (input.left.out_send_ready) {EXPECT_EQ(input.left.out_send_ready, dev->left_send_ready);}
        if (input.left.out_recv_ready) {EXPECT_EQ(input.left.out_recv_ready, dev->left_recv_ready);}

        if (input.right.out_send_data)  {EXPECT_EQ(input.right.out_send_data,  dev->right_send_data);}
        if (input.right.out_send_ready) {EXPECT_EQ(input.right.out_send_ready, dev->right_send_ready);}
        if (input.right.out_recv_ready) {EXPECT_EQ(input.right.out_recv_ready, dev->right_recv_ready);}
    }

    void applyInputs(const std::vector<TileIOInput>& inputs) {
        for (const auto& input : inputs) {
            applyInput(input);
        }
    }
};

TEST_F(TileIO, reset) {
    reset();
    TileIOInput input = {};
    input.out_recv_data = 0;
    input.out_recv_valid = 0;

    input.up.out_send_ready = 0;
    input.up.out_send_data = 0;
    input.up.out_recv_ready = 0;

    input.down.out_send_ready = 0;
    input.down.out_send_data = 0;
    input.down.out_recv_ready = 0;

    input.left.out_send_ready = 0;
    input.left.out_send_data = 0;
    input.left.out_recv_ready = 0;

    input.right.out_send_ready = 0;
    input.right.out_send_data = 0;
    input.right.out_recv_ready = 0;

    applyInput(input);
}

TEST_F(TileIO, WriteToUP) {
    reset();

    std::vector<TileIOInput> inputs = {{
        .in_loc = LOC_UP, .in_send_data = 0xab, .in_send = 1, .out_send_done = 0,
            .up = {
                .in_send_done = 0,
                .out_send_data = 0xab,
                .out_send_ready = 1,
                .out_recv_ready = 0
            }
        }, {
        .in_loc = LOC_UP, .in_send_data = 0xab, .in_send = 1, .out_send_done = 1,
            .up = {
                .in_send_done = 1,
                .out_send_data = 0xab,
                .out_send_ready = 1,
                .out_recv_ready = 0
            }
        }, {
        .in_loc = LOC_UP, .in_send_data = 0xab, .in_send = 0, .out_send_done = 0,
            .up = {
                .in_send_done = 0,
                .out_send_data = 0xab,
                .out_send_ready = 0,
                .out_recv_ready = 0
            }
        }
    };
    applyInputs(inputs);
}

TEST_F(TileIO, WriteToNil) {
    reset();

    std::vector<TileIOInput> inputs = {{
        .in_loc = LOC_NIL, .in_send_data = 0xff, .in_send = 1, .out_send_done = 1,
            .up = {
                .in_send_done = 0,
                .out_send_data = 0xff, // When NIL is selected, TileIO connects the data to
                .out_send_ready = 0,
                .out_recv_ready = 0
            }
        }, {
        .in_loc = LOC_UP, .in_send_data = 0xab, .in_send = 1, .out_send_done = 1,
            .up = {
                .in_send_done = 1,
                .out_send_data = 0xab,
                .out_send_ready = 1,
                .out_recv_ready = 0
            }
        }, {
        .in_loc = LOC_UP, .in_send_data = 0xab, .in_send = 0, .out_send_done = 0,
            .up = {
                .in_send_done = 0,
                .out_send_data = 0xab,
                .out_send_ready = 0,
                .out_recv_ready = 0
            }
        }
    };
    applyInputs(inputs);
}
