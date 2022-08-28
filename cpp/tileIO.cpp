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
        Word recv_data;
        Logic recv_valid;
        Logic send_done;

        // Module outputs:
        std::optional<Word> send_data;
        std::optional<Logic> send_ready;
        std::optional<Logic> recv_ready;
    };

    struct TileIOInput {
        Location loc;

        Word send_data;
        Logic send;
        std::optional<Logic> send_done;

        std::optional<Word> recv_data;
        Logic recv;
        std::optional<Logic> recv_valid;

        TileIF up, down, left, right;
    };


    void applyInput(const TileIOInput& input) {
        // Drive module inputs
        dev->loc = input.loc;
        dev->send = input.send;
        dev->recv = input.recv;
        dev->send_data = input.send_data;

        dev->up_send_done  = input.up.send_done;
        dev->up_recv_data  = input.up.recv_data;
        dev->up_recv_valid = input.up.recv_valid;

        dev->down_send_done  = input.down.send_done;
        dev->down_recv_data  = input.down.recv_data;
        dev->down_recv_valid = input.down.recv_valid;

        dev->left_send_done  = input.left.send_done;
        dev->left_recv_data  = input.left.recv_data;
        dev->left_recv_valid = input.left.recv_valid;

        dev->right_send_done  = input.right.send_done;
        dev->right_recv_data  = input.right.recv_data;
        dev->right_recv_valid = input.right.recv_valid;

        // Simulate a clock cycle
        tick();

        // Verify module outputs (only if expectations exist)
        if (input.send_done)  {EXPECT_EQ(input.send_done, dev->send_done);}
        if (input.recv_data)  {EXPECT_EQ(input.recv_data, dev->recv_data);}
        if (input.recv_valid) {EXPECT_EQ(input.recv_valid, dev->recv_valid);}

        if (input.up.send_data)  {EXPECT_EQ(input.up.send_data,  dev->up_send_data);}
        if (input.up.send_ready) {EXPECT_EQ(input.up.send_ready, dev->up_send_ready);}
        if (input.up.recv_ready) {EXPECT_EQ(input.up.recv_ready, dev->up_recv_ready);}

        if (input.down.send_data)  {EXPECT_EQ(input.down.send_data,  dev->down_send_data);}
        if (input.down.send_ready) {EXPECT_EQ(input.down.send_ready, dev->down_send_ready);}
        if (input.down.recv_ready) {EXPECT_EQ(input.down.recv_ready, dev->down_recv_ready);}

        if (input.left.send_data)  {EXPECT_EQ(input.left.send_data,  dev->left_send_data);}
        if (input.left.send_ready) {EXPECT_EQ(input.left.send_ready, dev->left_send_ready);}
        if (input.left.recv_ready) {EXPECT_EQ(input.left.recv_ready, dev->left_recv_ready);}

        if (input.right.send_data)  {EXPECT_EQ(input.right.send_data,  dev->right_send_data);}
        if (input.right.send_ready) {EXPECT_EQ(input.right.send_ready, dev->right_send_ready);}
        if (input.right.recv_ready) {EXPECT_EQ(input.right.recv_ready, dev->right_recv_ready);}
    }

    void applyInputs(const std::vector<TileIOInput>& inputs) {
        for (const auto& input : inputs) {
            applyInput(input);
        }
    }
};

TEST_F(TileIO, reset) {
    reset();
}
