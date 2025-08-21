//
// This software is dedicated to the public domain.
//
task adder33 (
	output reg [1:0] O_s,
	output reg [1:0] O_c,
	input [1:0] I_a,
	input [1:0] I_b,
	input [1:0] I_c
);
begin
	case ({I_a,I_b,I_c})
	6'b000000: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b000001: begin
		O_s = 2'b01;
		O_c = 2'b00;
	end
	6'b000010: begin
		O_s = 2'b10;
		O_c = 2'b00;
	end
	6'b000011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b000100: begin
		O_s = 2'b01;
		O_c = 2'b00;
	end
	6'b000101: begin
		O_s = 2'b10;
		O_c = 2'b01;
	end
	6'b000110: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b000111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b001000: begin
		O_s = 2'b10;
		O_c = 2'b00;
	end
	6'b001001: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b001010: begin
		O_s = 2'b01;
		O_c = 2'b10;
	end
	6'b001011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b001100: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b001101: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b001110: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b001111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b010000: begin
		O_s = 2'b01;
		O_c = 2'b00;
	end
	6'b010001: begin
		O_s = 2'b10;
		O_c = 2'b01;
	end
	6'b010010: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b010011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b010100: begin
		O_s = 2'b10;
		O_c = 2'b01;
	end
	6'b010101: begin
		O_s = 2'b00;
		O_c = 2'b01;
	end
	6'b010110: begin
		O_s = 2'b01;
		O_c = 2'b00;
	end
	6'b010111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b011000: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b011001: begin
		O_s = 2'b01;
		O_c = 2'b00;
	end
	6'b011010: begin
		O_s = 2'b10;
		O_c = 2'b00;
	end
	6'b011011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b011100: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b011101: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b011110: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b011111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b100000: begin
		O_s = 2'b10;
		O_c = 2'b00;
	end
	6'b100001: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b100010: begin
		O_s = 2'b01;
		O_c = 2'b10;
	end
	6'b100011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b100100: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b100101: begin
		O_s = 2'b01;
		O_c = 2'b00;
	end
	6'b100110: begin
		O_s = 2'b10;
		O_c = 2'b00;
	end
	6'b100111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b101000: begin
		O_s = 2'b01;
		O_c = 2'b10;
	end
	6'b101001: begin
		O_s = 2'b10;
		O_c = 2'b00;
	end
	6'b101010: begin
		O_s = 2'b00;
		O_c = 2'b10;
	end
	6'b101011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b101100: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b101101: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b101110: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b101111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110000: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110001: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110010: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110100: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110101: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110110: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b110111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111000: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111001: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111010: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111011: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111100: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111101: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111110: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	6'b111111: begin
		O_s = 2'b00;
		O_c = 2'b00;
	end
	endcase
end;
endtask
