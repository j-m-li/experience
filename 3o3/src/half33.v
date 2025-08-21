// 
// This software is dedicated to the public domain.
//

task half33 (
	output reg [1:0] O_s,
	output reg [1:0] O_c,
	input [1:0] I_a,
	input [1:0] I_b
);

begin
	sum33(O_s, I_a, I_b);
	cons33(O_c, I_a, I_b);
end

endtask

