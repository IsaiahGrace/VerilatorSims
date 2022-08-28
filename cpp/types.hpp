#ifndef CPP_TYPES_HPP_
#define CPP_TYPES_HPP_

#include <cstdint>

// This is a C++ translation of the file sv/types.vh
// See sv/type_asserter.sv for verification that the bit-widths here are correct.

enum Location : std::uint8_t {
    LOC_ACC   = 0,
    LOC_NIL   = 1,
    LOC_UP    = 2,
    LOC_DOWN  = 3,
    LOC_LEFT  = 4,
    LOC_RIGHT = 5,
    LOC_ANY   = 6,
    LOC_LAST  = 7
};

enum Opcode : std::uint8_t {
    OP_NOP = 0x00,  // NO-OP does nothing. Docs say it's assembled to ADD NIL. But I'm keeping it seperate
    OP_MOV = 0x01,  // MOVE <SRC> -> <DST>. The MOV instruction is split into two: MOV & MVI.
    OP_MVI = 0x02,  // MOVE <IMM> -> <DST>. IMM is a word, encoded into the instruction.
    OP_SWP = 0x03,  // SWAP ACC <-> BAK
    OP_SAV = 0x04,  // SAVE ACC -> BAK
    OP_ADD = 0x05,  // ACC = ACC + <SRC>
    OP_SUB = 0x06,  // ACC = ACC - <SRC>
    OP_NEG = 0x07,  // ACC = ACC * -1
    OP_JMP = 0x08,  // PC = <LABEL>
    OP_JEZ = 0x09,  // ACC == 0 ? PC = <LABEL> : PC++
    OP_JNZ = 0x0a,  // ACC != 0 ? PC = <LABEL> : PC++
    OP_JLZ = 0x0b,  // ACC  < 0 ? PC = <LABEL> : PC++
    OP_JRO = 0x0c   // PC = PC + ACC
};

typedef std::uint8_t  Logic;
typedef std::uint8_t  Word;
typedef std::uint8_t  Address;
typedef std::uint16_t Instruction;


#endif  // CPP_TYPES_HPP_
