`ifndef _TILE_VH_
`define _TILE_VH_

`include "types.vh"

interface tile_if;
    word  send_data;
    logic send_ready;
    logic send_done;

    word  recv_data;
    logic recv_ready;
    logic recv_valid;

    modport tile (
        input recv_data, recv_valid, send_done,
        output send_data, send_ready, recv_ready
    );

    modport mesh (
        input send_data, send_ready, recv_ready,
        output recv_data, recv_valid, send_done
    );
endinterface // tile_if

`endif // _TILE_VH_
