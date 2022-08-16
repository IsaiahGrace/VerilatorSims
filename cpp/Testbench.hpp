#include <cstdint>
#include <iostream>

#include "verilated.h"
#include "verilated_vcd_c.h"

// Thank you ZipCPU!
// https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html

template<class Module> class Testbench {
 private:
    VerilatedContext* context;
    VerilatedVcdC* vcd;
    std::string traceFile;

 public:
    uint64_t tickCount;
    Module* dev;

    Testbench(const std::string& testName) {
        context = new VerilatedContext;
        context->traceEverOn(true);
        context->debug(0);

        dev = new Module(context);

        vcd = new VerilatedVcdC;

        dev->trace(vcd, 99);

        // NOTE: open() MUST be called AFTER trace(), otherwise, dump() below segfaults...
        // VerilatedVcdC uses the const char* later at varius points! We need to ensure that the pointer is valid for the life of vcd!
        traceFile = "traces/" + testName + ".vcd";
        vcd->open(traceFile.c_str());

        // Generate a trace at the begining of time.
        dev->clk = 1;
        dev->eval();
        vcd->dump(context->time());

        tickCount = 0;
    }

    virtual ~Testbench() {
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
        return Verilated::gotFinish();
    }
};
