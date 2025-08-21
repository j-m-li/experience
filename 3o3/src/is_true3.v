// 
// This software is dedicated to the public domain.
//

task is_true3 (
	output reg [1:0] O_c,
	input [1:0] I_a
);

begin
	case (I_a)
	2'b10:    O_c = 2'b10; // -
	default:  O_c = 2'b10; // 0
	2'b01:    O_c = 2'b01;	// +
	endcase	
end

endtask

