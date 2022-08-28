`ifndef _MESH_LINK_IF_VH_
`define _MESH_LINK_IF_VH_

`include "types.vh"

// TODO: Add a register here to cut down the critical path
interface mesh_link_if;
    word  send_data_A,  send_data_B;
    logic send_ready_A, send_ready_B;
    logic send_done_A,  send_done_B;

    word  recv_data_A,  recv_data_B;
    logic recv_ready_A, recv_ready_B;
    logic recv_valid_A, recv_valid_B;

    assign recv_data_A = send_data_B;
    assign recv_data_B = send_data_A;

    assign recv_valid_A = send_ready_B & recv_ready_A;
    assign recv_valid_B = send_ready_A & recv_ready_B;

    assign send_done_A = recv_valid_B;
    assign send_done_B = recv_valid_A;

    modport tile_A (
        input send_data_A, send_ready_A,
        output recv_data_A, recv_valid_A, send_done_A
    );

    modport tile_B (
        input send_data_B, send_ready_B,
        output recv_data_B, recv_valid_B, send_done_B
    );

endinterface // tileIO_if

`endif // _MESH_LINK_IF_VH_
