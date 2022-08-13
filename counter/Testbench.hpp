#include <cstdint>
#include "verilated.h"

// Thank you ZipCPU!
// https://zipcpu.com/blog/2017/06/21/looking-at-verilator.html

template<class Module> class Testbench {
 public:
    uint64_t tickCount;
    Module* dev;

    Testbench() {
        dev = new Module;
        tickCount = 0;
    }

    virtual ~Testbench() {
        delete dev;
        dev = nullptr;
    }

    virtual void reset() {
        dev->nrst = 0;
        this->tick();
        dev->nrst = 1;
    }

    virtual void tick() {
        tickCount++;

        // Ensure combinational inputs which may have changed before tick() was called are evaluated.
        dev->clk = 0;
        dev->eval();

        dev->clk = 1;
        dev->eval();

        dev->clk = 0;
        dev->eval();

    }

    virtual bool done() {
        return Verilated::gotFinish();
    }
};
