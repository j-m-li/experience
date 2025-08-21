// 
// This software is dedicated to the public domain.
//

task any33 (
	output reg [1:0] O_c,
	input [1:0] I_a,
	input [1:0] I_b
);

begin
	case (I_a)
	2'b10: begin
		case (I_b)
		2'b10:   O_c = 2'b10; // --
		default: O_c = 2'b10; // -0
		2'b01:   O_c = 2'b00; // -+
		endcase
	end
	default: begin
		case (I_b)
		2'b10:   O_c = 2'b10; // 0-
		default: O_c = 2'b00; // 00
		2'b01:   O_c = 2'b01; // 0+
		endcase
	end
	2'b01: begin
		case (I_b)
		2'b10:   O_c = 2'b00; // +-
		default: O_c = 2'b01; // +0
		2'b01:   O_c = 2'b01; // ++
		endcase
	end
	endcase	
end

endtask

