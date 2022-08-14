module counter (
    input logic clk, nrst,
    output logic [7:0] count
    );

    always_ff @(posedge clk, negedge nrst) begin
        if (!nrst) begin
            count <= 0;
        end // if (!nrst)
        else begin
            count <= count + 1;
        end // else
    end

endmodule // counter
