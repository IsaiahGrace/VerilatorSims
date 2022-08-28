`include "types.vh"
`include "tile_if.vh"
`include "mesh_link_if.vh"

module type_asserter (
    /* verilator lint_off UNUSED */
    input logic clk, nrst
    );

    /* verilator lint_off UNUSED */
    address     _address;
    instruction _instruction;
    location    _location;
    opcode      _opcode;
    word        _word;

    // Elaboration time checks:

    // Check to make sure that the bit-widths are what we expect. We have lots of hard-coded
    // assumptions about these widths in our C++ code, so it makes sense to double check them here.
    generate
        if ($bits(_address)     !=  5) $error();
        if ($bits(_instruction) != 15) $error();
        if ($bits(_location)    !=  3) $error();
        if ($bits(_opcode)      !=  4) $error();
        if ($bits(_word)        !=  8) $error();
    endgenerate
endmodule // type_asserter
