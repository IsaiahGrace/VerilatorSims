#ifndef CPP_TESTBENCH_HPP_
#define CPP_TESTBENCH_HPP_

#include <cstdint>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

// Thank you ZipCPU!
// https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html

// This testbench assumes that the Module has "clk" and "nrst" signals.

template<class Module>
class Testbench: public ::testing::Test {
 protected:
    VerilatedContext* context;
    VerilatedVcdC* vcd;
    Module* dev;

    std::string traceFile;
    uint64_t tickCount;

    void SetUp() override {
        context = new VerilatedContext;
        context->traceEverOn(true);

        dev = new Module(context);
        vcd = new VerilatedVcdC;
        dev->trace(vcd, 99);

        // We'll create a .vcd file for each unit test, and name it traces/testSuite-testName.vcd
        const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
        std::ostringstream testName;
        testName << "traces/" << test_info->test_suite_name() << "-" << test_info->name() << ".vcd";
        traceFile = testName.str();

        // VerilatedVcdC uses the const char* later at varius points! We need to ensure that the
        // pointer is valid for the life of vcd!
        // NOTE: open() MUST be called AFTER trace(), otherwise, dump() below segfaults...
        vcd->open(traceFile.c_str());

        // Generate a trace at the begining of time.
        dev->clk = 1;
        dev->eval();
        vcd->dump(context->time());

        tickCount = 0;
    }

    void TearDown() override {
        // Evaluate and log the traces at the end of time
        dev->eval();
        vcd->dump(context->time());

        dev->final();
        vcd->close();

        delete dev;
        dev = nullptr;
        delete vcd;
        vcd = nullptr;
        delete context;
        context = nullptr;
    }

    void reset() {
        dev->nrst = 0;
        this->tick();
        dev->nrst = 1;
    }

    void tick() {
        tickCount++;

        // Ensure combinational inputs which may have changed before tick() was called are evaluated.
        dev->clk = 1;
        dev->eval();
        vcd->dump(context->time());

        // Low clock cycle
        context->timeInc(1);
        dev->clk = 0;
        dev->eval();
        vcd->dump(context->time());

        // High clock cycle
        context->timeInc(1);
        dev->clk = 1;
        dev->eval();

        // When tick() returns, time is at the instant AFTER the rising edge of the clock.
    }

    bool done() {
        return context->gotFinish();
    }
};

#endif  // CPP_TESTBENCH_HPP_
