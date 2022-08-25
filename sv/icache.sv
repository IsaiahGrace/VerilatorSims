`include "types.vh"

module icache (
   input logic clk, nrst,

   // Write signals
   input logic write,
   input address write_addr,
   input instruction write_data,

   // Read signals
   input  address read_addr,
   output instruction read_data
   );

   instruction[31:0] i_data, nxt_i_data;

   assign read_data = i_data[read_addr];

   always_comb begin
      nxt_i_data = i_data;      
      if (write) begin
         nxt_i_data[write_addr] = write_data;
      end
   end // always_comb

   always_ff @(posedge clk, negedge nrst) begin
      if (!nrst) begin
         i_data <= '0;
      end
      else begin
         i_data <= nxt_i_data;
      end
   end // always_ff @(posedge clk, negedge nrst)
endmodule // icache

