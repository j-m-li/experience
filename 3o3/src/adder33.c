
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	int i, j, m, p, s;
	printf("//\n");
	printf("// This software is dedicated to the public domain.\n");
	printf("//\n");
	printf("task adder33 (\n");
	printf("\toutput reg [1:0] O_s,\n");
	printf("\toutput reg [1:0] O_c,\n");
	printf("\tinput [1:0] I_a,\n");
	printf("\tinput [1:0] I_b,\n");
	printf("\tinput [1:0] I_c\n");
	printf(");\nbegin\n");
	printf("\tcase ({I_a,I_b,I_c})\n");
	for (i = 0; i <= 0x3F; i++) {
		printf("\t6'b");
		for (j = 5; j >= 0; j--) {
			printf("%c", ((i >> j) & 1) + '0');
		}
		printf(": begin\n");
		p = 0;
		m = 0;
		if (i & 0x01 && !(i & 0x02)) {
			p++;
		} else if (i & 0x02 && !(i & 0x01)) {
			m--;
		} else if (i & 0x03) {
			p = 1000;
		}
		if (i & 0x04 && !(i & 0x08)) {
			p++;
		} else if (i & 0x08 && !(i & 0x04)) {
			m--;
		} else if (i & 0x0C) {
			p = 1000;
		}
		if (i & 0x10 && !(i & 0x20)) {
			p++;
		} else if (i & 0x20 && !(i & 0x10)) {
			m--;
		} else if (i & 0x30) {
			p = 1000;
		}
		s = p + m;
		printf("\t\tO_s = 2'b");
		switch (s) {
		case 1:
		case -2:
			printf("01");
			break;
		case 2:
		case -1:
			printf("10");
			break;
		case 3:
		case -3:
			printf("00");
			break;
		default:
			printf("00");
		}
		printf(";\n\t\tO_c = 2'b");

		if (s > 3) {
			printf("00");
		} else if (s > 1) {
			printf("01");
		} else if (s < -1) {
			printf("10");
		} else {
			printf("00");
		}	
		printf(";\n\tend\n");
	}
	printf("\tendcase\n");
	printf("end;\nendtask\n");
	return 0;
}
