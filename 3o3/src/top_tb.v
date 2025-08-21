//
// This software is dedicated to the public domain.
//

`timescale 1ns / 1ns

`define assert(a, b) \
	case (a !== b) \
	1'b0: begin end \
	default: begin \
		$display("assertion FAILED %h !== %h", a, b); \
		$finish; \
	end \
	endcase

module test_bench;

reg clk;
reg rst;

wire [1:0] mosi;
wire [1:0] miso;
wire [1:0] sck;

reg [1:0] r  = 2'b11;
reg [1:0] c;
reg [3:0] s;
top dut (
	.I_clk(clk),
	.I_rst(rst),

	.O_mosi(mosi),
	.I_miso(miso),
	.O_sck(sck)
);


initial begin
	$monitor("%t: %h %b %b %b", $time, rst, mosi, miso, r);
	rst = 0;
	#100
	compare333_2(r, 4'b0010, 4'b0001);
	$display("R %b", r);
	#100
	adder333_2(s, c, 4'b0001, 4'b0010, 2'b01);
	$display("A %b %b", s, c);
	#100
	`assert(rst, 0);
	$finish;
end

initial begin
	clk = 0;
	#5
	forever clk = #5 ~clk;
end
endmodule

`include "top.v"

