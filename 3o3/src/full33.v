// 
// This software is dedicated to the public domain.
//

task full33 (
	output reg [1:0] O_s,
	output reg [1:0] O_c,
	input [1:0] I_a,
	input [1:0] I_b,
	input [1:0] I_c
);


reg [1:0] ab_s;
reg [1:0] ab_c;
reg [1:0] ab_c_c;

begin
	half33(ab_s, ab_c, I_a, I_b);
	half33(O_s, ab_c_c, ab_s, I_c);
	any33(O_c, ab_c_c, ab_c);
end

endtask

