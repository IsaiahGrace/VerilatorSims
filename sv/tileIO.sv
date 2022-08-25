`include "types.vh"
`include "tile_if.vh"

module tileIO (
    input logic clk, nrst,

    tile_if.tile up, down, left, right,

    input location loc,

    input word send_data,
    input logic send,
    output logic send_done,

    output word recv_data,
    input logic recv,
    output logic recv_valid
    );

    location last, nxt_last;

    // The 'external' and 'any' locations are ALWAYS one of the four cardinal locations
    location external;
    location any;

    assign nxt_last = (send | recv) ? external : last;

    always_comb begin : any_location
        casez ({up.recv_valid, down.recv_valid, left.recv_valid, right.recv_valid})
            4'b1???: any = LOC_UP;
            4'b01??: any = LOC_DOWN;
            4'b001?: any = LOC_LEFT;
            4'b0001: any = LOC_RIGHT;
            default: any = LOC_UP;
        endcase
        assert(any == LOC_UP || any == LOC_DOWN
            || any == LOC_LEFT || any == LOC_RIGHT);
    end // any_location

    always_comb begin : external_location
        case (loc)
            LOC_ACC  : external = LOC_UP;
            LOC_NIL  : external = LOC_UP;
            LOC_ANY  : external = any;
            LOC_LAST : external = last;
            default  : external = loc;
        endcase // loc
        assert(external == LOC_UP || external == LOC_DOWN
            || external == LOC_LEFT || external == LOC_RIGHT);
    end // external_location

    always_comb begin : drive_recv_valid
        if (loc == LOC_ANY) begin
            recv_valid = up.recv_valid | down.recv_valid | left.recv_valid | right.recv_valid;
        end
        else if (loc == LOC_NIL) begin
            recv_valid = 1;
        end
        else begin
            case (external)
                LOC_UP    : recv_valid = up.recv_valid;
                LOC_DOWN  : recv_valid = down.recv_valid;
                LOC_LEFT  : recv_valid = left.recv_valid;
                LOC_RIGHT : recv_valid = right.recv_valid;
                default   : recv_valid = 0;
            endcase // external
        end
    end // drive_recv_valid

    always_comb begin : drive_recv_data
        if (loc == LOC_NIL) begin
            recv_data = '0;
        end
        else if (recv) begin
            case (external)
                LOC_UP    : recv_data = up.recv_data;
                LOC_DOWN  : recv_data = down.recv_data;
                LOC_LEFT  : recv_data = left.recv_data;
                LOC_RIGHT : recv_data = right.recv_data;
                default   : recv_data = '0;
            endcase // external
        end
        else begin
            recv_data = '0;
        end
    end // drive_recv_data

    always_comb begin : drive_send_done
        send_done = '0;
        if (send) begin
            if (loc == LOC_NIL) begin
                send_done = 1;
            end
            else begin
                case (external)
                    LOC_UP    : send_done = up.send_done;
                    LOC_DOWN  : send_done = down.send_done;
                    LOC_LEFT  : send_done = left.send_done;
                    LOC_RIGHT : send_done = right.send_done;
                    default   : send_done = '0;
                endcase
            end
        end
    end // drive_send_done

    always_comb begin : drive_tile_if_outputs
        up.send_data  = '0;
        up.send_ready = '0;
        up.recv_ready = '0;

        down.send_data  = '0;
        down.send_ready = '0;
        down.recv_ready = '0;

        left.send_data  = '0;
        left.send_ready = '0;
        left.recv_ready = '0;

        right.send_data  = '0;
        right.send_ready = '0;
        right.recv_ready = '0;

        case (external)
            LOC_UP: begin
                up.send_data  = send_data;
                up.send_ready = send;
                up.recv_ready = recv;
            end

            LOC_DOWN: begin
                down.send_data  = send_data;
                down.send_ready = send;
                down.recv_ready = recv;
            end

            LOC_LEFT: begin
                left.send_data  = send_data;
                left.send_ready = send;
                left.recv_ready = recv;
            end

            LOC_RIGHT: begin
                right.send_data  = send_data;
                right.send_ready = send;
                right.recv_ready = recv;
            end

            // defaults are assigned above
            default:;
        endcase // external
    end // drive_tile_if_outputs

    always_ff @(posedge clk, negedge nrst) begin
        if (!nrst) begin
            last <= LOC_NIL;
        end
        else begin
            last <= nxt_last;
        end
    end // always_ff
endmodule // tileIO
