// 
// This software is dedicated to the public domain.
//
// https://homepage.cs.uiowa.edu/~jones/ternary/arith.shtml#comptwo
//


task compare333_2 (
	output reg [1:0] O_c,
	input [3:0] I_a,
	input [3:0] I_b
);

reg [1:0] c_0;
reg [1:0] c_1;

begin
	compare333_y(c_0, I_a[1:0], I_b[1:0]);
	compare333_y(c_1, I_a[3:2], I_b[3:2]);
	compare333_x(O_c, c_0, c_1);
end
endtask

// meaning 
// - = <
// 0 = =
// + = >
task compare333_x (
	output reg [1:0] O_c,
	input [1:0] I_a,
	input [1:0] I_b
);

begin
	case (I_b)
	2'b10: begin
		case (I_a)
		2'b10:   O_c = 2'b10; // --
		default: O_c = 2'b10; // -0
		2'b01:   O_c = 2'b10; // -+
		endcase
	end
	default: begin
		case (I_a)
		2'b10:   O_c = 2'b10; // 0-
		default: O_c = 2'b00; // 00
		2'b01:   O_c = 2'b01; // 0+
		endcase
	end
	2'b01: begin
		case (I_a)
		2'b10:   O_c = 2'b01; // +-
		default: O_c = 2'b01; // +0
		2'b01:   O_c = 2'b01; // ++
		endcase
	end
	endcase	
end
endtask


// meaning 
// - = <
// 0 = =
// + = >
task compare333_y (
	output reg [1:0] O_c,
	input [1:0] I_a,
	input [1:0] I_b
);

begin
	case (I_b)
	2'b10: begin
		case (I_a)
		2'b10:   O_c = 2'b00; // --
		default: O_c = 2'b01; // -0
		2'b01:   O_c = 2'b01; // -+
		endcase
	end
	default: begin
		case (I_a)
		2'b10:   O_c = 2'b10; // 0-
		default: O_c = 2'b00; // 00
		2'b01:   O_c = 2'b01; // 0+
		endcase
	end
	2'b01: begin
		case (I_a)
		2'b10:   O_c = 2'b10; // +-
		default: O_c = 2'b10; // +0
		2'b01:   O_c = 2'b00; // ++
		endcase
	end
	endcase	
end
endtask

