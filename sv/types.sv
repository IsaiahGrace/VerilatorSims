// These are the types used by the TIS-100 CPU tiles
`timescale 10ns/10ns

// NOTE: BAK is not included here becasue it is not a valid destination for the MOV instruction
typedef enum logic [2:0] {
    LOC_ACC   = 3'h0,
    LOC_NIL   = 3'h1,
    LOC_UP    = 3'h2,
    LOC_DOWN  = 3'h3,
    LOC_LEFT  = 3'h4,
    LOC_RIGHT = 3'h5,
    LOC_ANY   = 3'h6,
    LOC_LAST  = 3'h7
} location;


typedef enum logic [3:0] {
    OP_NOP = 4'h0,  // NO-OP does nothing. Docs say it's assembled to ADD NIL. But I'm keeping it seperate
    OP_MOV = 4'h1,  // MOVE <SRC> -> <DST>. The MOV instruction is split into two: MOV & MVI.
    OP_MVI = 4'h2,  // MOVE <IMM> -> <DST>. IMM is a word, encoded into the instruction.
    OP_SWP = 4'h3,  // SWAP ACC <-> BAK
    OP_SAV = 4'h4,  // SAVE ACC -> BAK
    OP_ADD = 4'h5,  // ACC = ACC + <SRC>
    OP_SUB = 4'h6,  // ACC = ACC - <SRC>
    OP_NEG = 4'h7,  // ACC = ACC * -1
    OP_JMP = 4'h8,  // PC = <LABEL>
    OP_JEZ = 4'h9,  // ACC == 0 ? PC = <LABEL> : PC++
    OP_JNZ = 4'ha,  // ACC != 0 ? PC = <LABEL> : PC++
    OP_JLZ = 4'hb,  // ACC  < 0 ? PC = <LABEL> : PC++
    OP_JRO = 4'hc   // PC = PC + ACC
} opcode;

typedef logic [7:0] word;
typedef logic [4:0] address;

typedef struct packed {
    location dst;
    location src;
} mov_type;

typedef struct packed {
    location dst;
    word imm;
} mvi_type;

typedef struct packed {
    location pad;
    location src;
} arith_type;

typedef struct packed {
    opcode op;
    union packed {
        mov_type mov;
        mvi_type mvi;
        arith_type arr;
        address label;
    } data;
} instruction;
