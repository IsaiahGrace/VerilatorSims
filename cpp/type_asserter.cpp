#include "verilated.h"
#include "gtest/gtest.h"

#include "Testbench.hpp"

#include "Vtype_asserter.h"

class Type_asserter: public Testbench<Vtype_asserter> {
 protected:
    void SetUp() override {
        Testbench<Vtype_asserter>::SetUp();
        context->fatalOnError(false);
    }

    void TearDown() override {
        Testbench<Vtype_asserter>::TearDown();
    }
};

// There are no simulation time checks right now.
/*
TEST_F(Type_asserter, simulationTimeErrors) {
    dev->eval();
    EXPECT_FALSE(context->gotFinish());
}
*/
