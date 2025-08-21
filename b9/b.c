
/* BASE 9 (+/-4) in balanced ternary logic */
 
#include <stdio.h>
#include <stdlib.h>

int argc;
char **argv;
FILE *in;
FILE *out;
int line = 1;
int column = 1;
int mode = 0;

int ascii2trit(int c, int hig)
{
	int a = -10000;
	int b = -10000;
	switch (c) {
	case '4': a = 1; b = 1; break;
	case '3': a = 0; b = 1; break;
	case '2': a = -1; b = 1; break;
	case '1': a = 1; b = 0; break;
	case '0': a = 0; b = 0; break;
	case 'a': a = -1; b = 0; break;
	case 'b': a = 1; b = -1; break;
	case 'c': a = 0; b = -1; break;
	case 'd': a = -1; b = -1; break;
	}
	if (hig) {
		return b;
	}
	return a;
}

int int2tri(int x, char *buf)
{
	if (mode) {
		/* to ascii */
		buf[0] = ' ';
		switch (x) {
		case 4:  buf[0] = '4'; break; /* 4 */
		case 3:  buf[0] = '3'; break; /* 3 */
		case 2:  buf[0] = '2'; break; /* 2 */
		case 1:  buf[0] = '1'; break; /* 1 */
		case 0:  buf[0] = '0'; break; /* 0 */
		case -1: buf[0] = 'a'; break; /* a */
		case -2: buf[0] = 'b'; break; /* b */
		case -3: buf[0] = 'c'; break; /* c */
		case -4: buf[0] = 'd'; break; /* d */
		case '\n': buf[0] = '\n'; break;
		case ' ': buf[0] = ' '; break;
		default: printf("errorx/%d/\n", x);
		}
		return 1;
	}
	/* TRITII: 43210abcd space newline */
	/* to UTF-8 */
	buf[0] = 0xE2;
	buf[1] = 0xA0;
	buf[2] = 0xBF;
	switch (x) {
	case 4:  buf[2] = 0x8E; break; /* 4 */
	case 3:  buf[2] = 0x8C; break; /* 3 */
	case 2:  buf[2] = 0x95; break; /* 2 */
	case 1:  buf[2] = 0x9C; break; /* 1 */
	case 0:  buf[2] = 0x80; break; /* 0 */
	case -1: buf[2] = 0xA3; break; /* a */
	case -2: buf[2] = 0xAA; break; /* b */
	case -3: buf[2] = 0xA1; break; /* c */
	case -4: buf[2] = 0xB1; break; /* d */
	case '\n': buf[0] = '\n'; return 1;
	default: buf[0] = '_'; return 1;
	}
	return 3;
}

void int2txt(int x, char *buf, int *len)
{
	static int vv = 387420489; 
	int l = 0;
	int bl = *len;
	int d;
	int k;
	int v;
/*	
	vv = 81;
	v = 81 * 9;
	d = 4;
	k = 4 * 9 + 4;
	while (k > d && v > vv)	{
		d = k;
		k = d * 9 + 4;
		vv = v;
		v = v * 9;
	}

	printf(">%d\n", vv);
*/

	v = vv;
	if (x > v || x < -v) {
		return;
	}
	while (v > 1) {
		k = (v-1) / 2;
		if (x > 0 && x > k) {
			break;
		} else if (x < 0 && -x > k) {
			break;
		}
		v = v / 9;
	}
	while (v > 1 && bl >= 3) {
		k = ((v)-1) / 2;
		/*printf("x%d(k%d)", x,k);*/
		if (x > 0 && x > k) {
			d = (x) / v;
			x = x - d * v;
			if (x > k) {
				x = x - v;
				d++;
			}
			l += int2tri(d, buf + l);
		} else if (x < 0 && -x > k) {
			d = (-x) / v;
			x = x + d * v;
			if (-x > k) {
				x = x + v;
				d++;
			}
			l += int2tri(-d, buf + l);
		} else {
			l += int2tri(0, buf + l);
		}
		/*printf("x%d v%d d%d\n", x, v, d);*/
		bl = bl - 3;
		v = v / 9;
	}
	/*printf(">v%d x%d\n", v, x); */
	l += int2tri(x, buf + l);
	l += int2tri(' ', buf + l);

	*len = l;
}

void printn(int x)
{
	char buf[80];
	int l = 80;
	int2txt(x, buf, &l);
	fwrite(buf, 1, l, stdout);
}

void quit(int ec)
{
	fclose(in);
	fclose(out);
	exit(ec);
}

int setlabel(char *label, int len, int offset, int newoffset)
{
	int i;
	if ( newoffset > offset) {
		offset = newoffset;
	}
	for (i = 0; i < len; i++) {
		fwrite(label + i, 1, 1, stdout);
	}
	fwrite(" ", 1, 1, stdout);
	printf("%d", offset);
	fwrite("\n", 1, 1, stdout);

	return offset;
}

void loop()
{
	static char buf[512];
	static int idx = 0;
	static int end = 0;
	static int function = 0;
	static int value[18];
	static int tryte[3];
	static int state = 0;
	static int address = 0;
	static int trit = 0;
	static int bit = 0;
	static int vi = 18;
	static int ti = 3;
	static char obu[1];
	static char spaces = 0;
	static int labelmode = 0;
	static int offset = 0;
	static char label[80];
	static int li = 0;
	static int newoffset = -1;
	int l;
	int i;
	int t;

	if (idx == end) {
		l = fread(buf, 1, 512, in);	
		if (l < 1) {
			state = 255;
		} else {
			end = l;
		}
		idx = 0;
		return;
	}	
	/*printn(state);
	fwrite("\n", 1, 1, stdout);	
	*/
	switch (state) {
	case 0: /* read tryte */
		if (buf[idx] == ' ') {
			spaces++;
			if (spaces > 1) {
				if (li) {
					offset = setlabel(label, 
							li, offset,newoffset);
					li = 0;
					newoffset = -1;
				}
				spaces = 0;
				state = 3;
				labelmode &= ~1;
				return;
			} else if (li) {
				newoffset = 0;
			}
		} else if (buf[idx] == '\n') {
			column = 0;
			spaces = 0;
			labelmode &= ~1;
			if (li) {
				offset = setlabel(label, li, offset, newoffset);
				newoffset = -1;
				li = 0;
			}
		} else {
			spaces = 0;
			if (column == 1) {
				labelmode |= 1;
				li = 0;
			}
		}
		if ((labelmode & 1) == 0) {
			t = ascii2trit(buf[idx], trit);
		} else {
			t = -1000;
			if (li < sizeof(label) && newoffset < 0) {
				label[li] = buf[idx];
				li++;
			} else {
				newoffset *= 3;
				newoffset += ascii2trit(buf[idx], 1);
				newoffset *= 3;
				newoffset += ascii2trit(buf[idx], 0);
			}
		}
		if (t >= -1 && t <= 1) {
			ti--;
			tryte[ti] = t;
		       	if (trit == 0) {
				trit = 1;
			} else {
				idx++;
				column++;
				trit = 0;
			}	
		} else {
			idx++;
			column++;
			trit = 0;
			if (ti == 3) {
				switch (state) {
				case 0:
					break;
				}
				return;
			}
			while (ti > 0) {
				tryte[ti-1] = tryte[ti];
				ti--;
			}
		}	
		
		if (ti == 0) {
			ti = 3;
			switch (state) {
			case 0:
				state = 1;
				break;
			}
		}
		break;
	case 1:  /* process tryte */
		if ((labelmode & 1) == 1) {
			state = 0;
			return;
		}
		
		for (i = 0; i < 3; i++) {
			if (bit == 0) {
				obu[0] = 0;
			}
			if (tryte[i] == -1) {
				obu[0] |= 2 << bit;
			} else if (tryte[1] == 1) {
				obu[0] |= 1 << bit;

			}
			bit += 2;
			if (bit == 8) {
				bit = 0;
				fwrite(obu, 1, 1, out);	
			}
		}
		offset++;
		state = 0;
		break;
	case 2: 
		state++;
		break;
	case 3: /* comment */
		printf("%c", buf[idx]);
		if (buf[idx] == '\n') {
			line++;
			column = 0;
			state = 0;
		}
		idx++;
		column++;
		break;
	case 4: 
		state++;
		break;
	case 5: 
		state++;
		break;
	case 255:
		if (bit > 0) {
			while (bit != 8) {
				obu[0] |= 3 << bit;
				bit += 2;
			}
			fwrite(obu, 1, 1, out);	
		}
	
		quit(0);
		break;
	default:	
		state++;
		if (state > 1000) {
			quit(0);
		}
	}
}

void setup()
{
	if (argc != 3) {
		exit(-1);
	}
	in = fopen(argv[1], "r");
	if (!in) {
		exit(-2);
	}
	out = fopen(argv[2], "w");
	if (!out) {
		exit(-3);
	}
	printn(-4);
	printn(-3);
	printn(-2);
	printn(-1);
	printn(0);
	printn(1);
	printn(2);
	printn(3);
	printn(4);
	fwrite("\n", 1, 1, stdout);	
	mode = 1;
}

int main(int argc_, char *argv_[])
{
	argc = argc_;
	argv = argv_;
	setup();
	while (1) {
		loop();
	}
	return -1;
}

