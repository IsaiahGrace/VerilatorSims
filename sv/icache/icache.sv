`timescale 10ns/10ns
`include "types.sv"

module icache (
               input wire clk, nrst,

               // Write signals
               input wire write,
               input      address write_addr,
               input      word write_data,

               // Read signals
               input      address read_addr,
               output     word read_data
               );
   word[31:0] i_data, nxt_i_data;

   assign read_data = i_data[read_addr];

   always_comb begin
      nxt_i_data = i_data;      
      if (write) begin
         nxt_i_data[write_addr] = write_data;
      end
   end

   always_ff @(posedge clk, negedge nrst) begin
      if (!nrst) begin
         i_data <= '0;
      end
      else begin
         i_data <= nxt_i_data;
      end
   end
endmodule // icache

