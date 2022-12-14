`include "types.vh"
`include "tile_if.vh"

// NOTE: This module CANNOT be a top-level module in a verilator simulation because it includes an interface port. Use tile_top to simulate a single tile instead.
module tile (
    input logic clk, nrst,

    // For communication with other tiles on the mesh.
    tile_if.tile up, down, left, right,
    // input recv_data, recv_valid, send_done,
    // output send_data, send_ready, recv_ready

    // For programming the tile after power up.
    input logic icache_write,
    input address icache_addr,
    input instruction icache_data
    );

    address pc;
    instruction instr;
    word acc;

    // Register file inputs
    wire RF_write, RF_swap, RF_save;
    word RF_write_data;

    // TileIO wires
    location IO_loc;
    word IO_send_data;
    logic IO_send, IO_send_done;
    word IO_recv_data;
    logic IO_recv, IO_recv_valid;

    // Module instances:

    tileIO TILEIO (
        .clk(clk),
        .nrst(nrst),
        .up(up),
        .down(down),
        .left(left),
        .right(right),
        .loc(IO_loc),
        .send_data(IO_send_data),
        .send(IO_send),
        .send_done(IO_send_done),
        .recv_data(IO_recv_data),
        .recv(IO_recv),
        .recv_valid(IO_recv_valid)
    );

    icache ICACHE (
        .clk(clk),
        .nrst(nrst),
        .read_addr(pc),
        .read_data(instr),
        .write(icache_write),
        .write_addr(icache_addr),
        .write_data(icache_data)
    );

    register_file RF (
        .clk(clk),
        .nrst(nrst),
        .write(RF_write),
        .swap(RF_swap),
        .save(RF_save),
        .write_data(RF_write_data),
        .acc(acc)
    );

endmodule // tile
