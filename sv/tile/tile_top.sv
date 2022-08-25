`include "types.vh"
`include "tile_if.vh"

module tile_top (
    input logic clk, nrst,

    // For programming the tile after power up.
    input logic icache_write,
    input address icache_addr,
    input instruction icache_data,

    // UP
    output word  up_send_data,
    output logic up_send_ready,
    input  logic up_send_done,
    input  word  up_recv_data,
    output logic up_recv_ready,
    input  logic up_recv_valid,

    // down
    output word  down_send_data,
    output logic down_send_ready,
    input  logic down_send_done,
    input  word  down_recv_data,
    output logic down_recv_ready,
    input  logic down_recv_valid,

    // left
    output word  left_send_data,
    output logic left_send_ready,
    input  logic left_send_done,
    input  word  left_recv_data,
    output logic left_recv_ready,
    input  logic left_recv_valid,

    // right
    output word  right_send_data,
    output logic right_send_ready,
    input  logic right_send_done,
    input  word  right_recv_data,
    output logic right_recv_ready,
    input  logic right_recv_valid
    );

    // Interfaces:
    tile_if up, down, left, right;

    // We must manually unrap these interfaces because verilator can't handle an interface in the top level.
    assign up.mesh.recv_data = up_recv_data;
    assign up.mesh.recv_valid = up_recv_valid;
    assign up.mesh.send_done = up_send_done;
    assign up_send_data = up.mesh.send_data;
    assign up_send_ready = up.mesh.send_ready;
    assign up_recv_ready = up.mesh.recv_ready;

    assign down.mesh.recv_data = down_recv_data;
    assign down.mesh.recv_valid = down_recv_valid;
    assign down.mesh.send_done = down_send_done;
    assign down_send_data = down.mesh.send_data;
    assign down_send_ready = down.mesh.send_ready;
    assign down_recv_ready = down.mesh.recv_ready;

    assign left.mesh.recv_data = left_recv_data;
    assign left.mesh.recv_valid = left_recv_valid;
    assign left.mesh.send_done = left_send_done;
    assign left_send_data = left.mesh.send_data;
    assign left_send_ready = left.mesh.send_ready;
    assign left_recv_ready = left.mesh.recv_ready;

    assign right.mesh.recv_data = right_recv_data;
    assign right.mesh.recv_valid = right_recv_valid;
    assign right.mesh.send_done = right_send_done;
    assign right_send_data = right.mesh.send_data;
    assign right_send_ready = right.mesh.send_ready;
    assign right_recv_ready = right.mesh.recv_ready;


    tile TILE (
        .clk(clk),
        .nrst(nrst),

        .up(up.tile),
        .down(down.tile),
        .left(left.tile),
        .right(right.tile),

        .icache_write(icache_write),
        .icache_addr (icache_addr),
        .icache_data (icache_data)
    );

endmodule // tile_top
