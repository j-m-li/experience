// 
// This software is dedicated to the public domain.
//
// https://homepage.cs.uiowa.edu/~jones/ternary/arith.shtml#addahead
//

task adder333_2 (
	output reg [3:0] O_s,
	output reg [1:0] O_c,
	input [3:0] I_a,
	input [3:0] I_b,
	input [1:0] I_c
);

reg [1:0] c_10;

begin
	adder33(O_s[1:0], c_10, I_a[1:0], I_b[1:0], I_c); 
	adder33(O_s[3:2], O_c, I_a[3:2], I_b[3:2], c_10); 
end
endtask
