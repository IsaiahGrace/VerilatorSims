`include "types.sv"

// A simple 2 word register file for the TIS-100 arch.
module RF (
	input logic clk, nrst,
	input logic write, swap, save,
	input word write_data,
	output word acc
	);

	word nxt_acc, nxt_bak;
	word bak;

	always_comb begin
		nxt_acc = acc;
		nxt_bak = bak;

		// Design choice, if two inputs are enabled at the same time, the register file will do nothing.

		// Write
		if (write & !swap & !save) begin
			nxt_acc = write_data;
		end

		// Swap
		if (!write & swap & !save) begin
			nxt_acc = bak;
			nxt_bak = acc;
		end

		// Save
		if (!write & !swap & save) begin
			nxt_bak = acc;
		end
	end // always_comb

	always_ff @(posedge clk, negedge nrst) begin
		if (!nrst) begin
			acc <= '0;
			bak <= '0;
		end
		else begin
			acc <= nxt_acc;
			bak <= nxt_bak;
		end
	end // always_ff @(posedge clk, negedge nrst)

endmodule // RF
