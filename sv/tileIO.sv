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

    location last;
    $error("read the issue description")
    // Okay, so we have a problem with the tile interface as it stands with regard to the LOC_ANY
    // location. When we want to recieve from any location, things work okay because we can see who
    // is tying to send us data (using the recv_valid) signal. But when we are tying to send to any
    // location, we curently don't have any idea who's listening. As I see it now we have two options:
    //
    // 1. Broadcast the data to all the directions and end the transaction as soon as ONE of them
    //    responds with send_done. The issue with this is that data might be duplicated. If two
    //    neibours are trying to read from us, we'll send the data to both. This could be a design
    //    feature or a flaw, I'm not sure yet.
    // 2. Create a new signal called 'send_ready' or something similar wich goes high when a device
    //    is trying to recieve data from us. The issue I see with this is that we'll only use this
    //    signal in the case of a write to the LOC_ANY location.
    //
    // I should investigate the game and see how the "real" TIS-100 behaves.

    // The 'external' and 'any' locations are ALWAYS one of the four cardinal locations
    location external;
    location any;

    always_comb begin : any_location
        any = LOC_NIL;
        if (recv) begin
            casez ({up.recv_valid, down.recv_valid, left.recv_valid, right.recv_valid})
                4'b1???: any = LOC_UP;
                4'b01??: any = LOC_DOWN;
                4'b001?: any = LOC_LEFT;
                4'b0001: any = LOC_RIGHT;
                default: any = LOC_UP;
            endcase
        end
        else if (send) begin
            casez ({up.recv_valid, down.recv_valid, left.recv_valid, right.recv_valid})
                4'b1???: any = LOC_UP;
                4'b01??: any = LOC_DOWN;
                4'b001?: any = LOC_LEFT;
                4'b0001: any = LOC_RIGHT;
                default: any = LOC_UP;
            endcase
        end
        assert(any == LOC_UP || any == LOC_DOWN
            || any == LOC_LEFT || any == LOC_RIGHT);
    end // any_location

    always_comb begin : external_location
        case (loc)
            LOC_ACC  : external = LOC_NIL;
            //LOC_NIL  : external = LOC_UP;
            LOC_ANY  : external = any;
            LOC_LAST : external = last;
            default  : external = loc;
        endcase // loc
        assert(external == LOC_UP || external == LOC_DOWN
            || external == LOC_LEFT || external == LOC_RIGHT
            || external == LOC_NIL);
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
                LOC_NIL   : recv_valid = 1;
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
                LOC_NIL   : recv_data = '0;
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
            last <= (send | recv) ? external : last;
        end
    end // always_ff
endmodule // tileIO
