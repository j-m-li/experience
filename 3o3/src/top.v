// 
// This software is dedicated to the public domain.
//

`include "ternary.v"

module top (
	input I_clk,
	input I_rst,

	output reg [1:0] O_mosi,
	output reg [1:0] O_sck,
	input [1:0] I_miso
);

reg [1:0] x;
reg [1:0] a;
reg [1:0] b;
reg [1:0] s;
reg [1:0] c;
always @(posedge I_clk) begin
	case (I_rst)
	1'b1: begin
		O_mosi <= 0;
	end
	default: begin
		O_mosi <= {1'b1, I_clk};
		neg3(O_sck, x);
		full33(s, c, a, x, 2'b00);
	//	O_sck <= x;
	end
	endcase
end

always @(*) begin
	case (I_rst)
	1'b1: begin
		x <= ZERO;
	end
	1'b0: begin
		case (I_miso)
		PLUS: x <= PLUS;
		MINUS: x <= PLUS;
		default: x <= ZERO;
		endcase
	end
	endcase	
end

endmodule

