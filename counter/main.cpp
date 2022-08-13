#include <iostream>

#include "verilated.h"

#include "Testbench.hpp"

#include "Vcounter.h"

int main(int argc, char** argv) {
	Verilated::commandArgs(argc, argv);

    Testbench<Vcounter> tb;

    tb.reset();

    for (int i = 0; i < 300; i++) {
    	tb.tick();
	    std::cout << "Tick Count: [" << tb.tickCount << "] counter: [" << (int) tb.dev->count << "]" << std::endl;
    }

    tb.reset();

    return 0;
}
