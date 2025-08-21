// 
// This software is dedicated to the public domain.
//

task decoder3 (
	output reg [1:0] O_n,
	output reg [1:0] O_z,
	output reg [1:0] O_p,
	input [1:0] I_a
);

begin
	case (I_a)
	2'b10: begin // -
		O_n = 2'b01;
		O_z = 2'b10;
		O_p = 2'b10;
	end
	default: begin  // 0
		O_n = 2'b10;
		O_z = 2'b01;
		O_p = 2'b10;
	end
	2'b01: begin // +
		O_n = 2'b10;
		O_z = 2'b00;
		O_p = 2'b11;
	end
	endcase	
end

endtask

