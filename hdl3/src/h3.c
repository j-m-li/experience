/*
		 ZeASM programing language

 As sun, earth, water & wind, this work is neither ours nor yours.

	      MMXXV May 29 PUBLIC DOMAIN by JML

    The authors and contributors disclaim copyright, patents
	   and all related rights to this software.

 Anyone is free to copy, modify, publish, use, compile, sell, or
 distribute this software, either in source code form or as a
 compiled binary, for any purpose, commercial or non-commercial,
 and by any means.

 The authors waive all rights to patents, both currently owned
 by the authors or acquired in the future, that are necessarily
 infringed by this software, relating to make, have made, repair,
 use, sell, import, transfer, distribute or configure hardware
 or software in finished or intermediate form, whether by run,
 manufacture, assembly, testing, compiling, processing, loading
 or applying this software or otherwise.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT OF ANY PATENT, COPYRIGHT, TRADE SECRET OR OTHER
 PROPRIETARY RIGHT.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR
 ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*
 Must run on a CH32x035G8R6 $0.40
 (32bit RISC-V, 62kB Flash, 20kB SRAM, USB host/device, 48MHz)

 id = ((class & 0xFF) << 24) | \
	((size & 0xFF) << 16) | \
	(label & 0xFFFF);
 local variables and function parameters are static.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define byte unsigned char
#define var long

#define MAX_ID_LEN 70
#define MAX_REF 40
#define MAX_SIZE 255
#define MAX_LABEL 65535
#define SIZE_OF_VAR sizeof(var)

byte class_name[MAX_ID_LEN + 1];
byte func_name[MAX_ID_LEN + 1];
byte ret_name[MAX_REF][MAX_ID_LEN + 1];
var ret_start[MAX_REF];
var ret_end[MAX_REF];
var nb_ret;
byte var_name[MAX_ID_LEN + 1];
byte value[MAX_SIZE + 1];
byte input[512];
var ahead = 0;
byte tmp[MAX_ID_LEN + 1];
byte ref_name[MAX_REF][MAX_ID_LEN + 1];
byte ref_class[MAX_REF][MAX_ID_LEN + 1];
byte class_new[MAX_REF][MAX_ID_LEN + 1];
byte class_new_class[MAX_REF][MAX_ID_LEN + 1];
byte class_var[MAX_REF][MAX_ID_LEN + 1];
var nb_class_new;
var nb_class_var;
var line;
var nb_methods;
var nb_ref;
var indent = 0;

var comment();
var keyword();
var args();
var get_class_name(byte *first);
var is_member(byte *id);

var next() {
	var t = ahead;
	ahead = getchar();
	return t;
}

var error(char *str) {
	printf("#error at line %ld (%c)  %s\n", line, (int)ahead, str);
	fflush(stdout);
	exit(-1);
}

var identifier(byte *buf) {
	var i = 0;
	while (i < MAX_ID_LEN && ahead != EOF) {
		if ((ahead >= 'a' && ahead <= 'z')) {
			buf[i] = ahead;
			i++;
		} else {
			break;
		}
		next();
	}
	if (i < 1) {
		error("identifier");
	}
	buf[i] = 0;
	return (var)buf;
}

var identifiera(byte *buf, var *start, var *end) 
{
	identifier(buf);
	*start = 0;
	*end = -1;
	if (ahead >= '0' && ahead <= '9') {
		while (ahead >= '0' && ahead <= '9') {
			*start *= 10;
			*start += ahead - '0';
			next();
		}
		if (ahead != '.') {
			error("missing .");
		}
		next();
		if (ahead != '.') {
			error("missing .");
		}
		next();
		if (ahead < '0' || ahead > '9') {
			error("number expected");
		}
		*end = 0;
		while (ahead >= '0' && ahead <= '9') {
			*end *= 10;
			*end += ahead - '0';
			next();
		}

	}
	return (var)buf;
}

var kcmp(char *k, byte *tmp) {
	byte *key = (byte *)k;
	var i = 0;
	while (tmp[i]) {
		if (tmp[i] != key[i]) {
			return tmp[i] - key[i];
		}
		i++;
	}
	if (!key[i]) {
		return 0;
	}
	while (key[i] && ahead != EOF && i < MAX_ID_LEN) {
		if (ahead >= 'a' && ahead <= 'z') {
			if (ahead == key[i]) {
				tmp[i] = ahead;
				next();
				i++;
			} else {
				break;
			}
		} else {
			break;
		}
	}
	tmp[i] = 0;
	if (!key[i]) {
		if (ahead >= 'a' && ahead <= 'z') {
			return 1;
		}
		return 0;
	}
	return -1;
}

var spaces() {
	while (ahead != EOF) {
		switch (ahead) {
		case '\n':
			line++;
		case ' ':
		case '\t':
		case '\r':
			break;
		case '?':
			next();
			if (ahead == '\r') {
				next();
			}
			if (ahead == '\n') {
				line++;
			}
			break;
		case '!':
			comment();
			continue;
			break;
		default:
			return 0;
		}
		next();
	}
	return 0;
}

var tabs() {
	var i;
	for (i = 0; i < indent; i++) {
		printf("\t");
	}
	return 0;
}

var hex(var n) {
	if (n >= 0 && n <= 9) {
		return '0' + n;
	} else if (n >= 10 && n <= 15) {
		return 'A' - 10 + n;
	}
	return -1;
}

var byte_string(byte *buf, var offset) {
	var i;
	var l = 0;
	var n = 0;
	if (ahead != '\'') {
		error("bytes");
		return 0;
	}
	next();
	i = offset;
	while (i < (MAX_SIZE - 8) && ahead != EOF) {
		if (ahead == '\'') {
			buf[i] = 0;
			next();
			return l;
		}
		buf[i] = '\\';
		i++;
		buf[i] = 'x';
		i++;
		if (ahead == '-') {
			n = 0;
		} else if (ahead >= 'a' && ahead <= 'o') {
			n = ahead - 'a' + 1;
		} else {
			error("syntax");
		}
		buf[i] = hex(n);
		i++;
		next();
		if (ahead == '-') {
			n = 0;
		} else if (ahead >= 'a' && ahead <= 'o') {
			n = ahead - 'a' + 1;
		} else {
			error("syntax");
		}
		buf[i] = hex(n);
		i++;
		next();
		l++;
	}
	error("bytes no end");
	return 0;
}

var string(byte *buf, var offset) {
	var i;
	var c;
	if (ahead != '"') {
		error("string");
		return 0;
	}
	next();
	i = offset;
	while (i < MAX_SIZE && (c = next()) != EOF) {
		if (c == '"') {
			buf[i] = 0;
			return (var)buf;
		}
		buf[i] = c;
		i++;
	}
	error("string no end");
	return 0;
}

var number() {
	var i;
	var c;
	var s = 1;
	if (ahead == '-') {
		next();
		s = -1;
	}
	if (ahead < '0' || ahead > '9') {
		error("number");
		return 0;
	}
	i = 0;
	c = 0;
	while (i < 20 && (ahead != EOF)) {
		if (ahead < '0' || ahead > '9') {
			if (s < 1) {
				return -c;
			}
			return c;
		}
		c = (c * 10) + (ahead - '0');
		i++;
		next();
	}
	error("num too big");
	return 0;
}

var include() {
	byte *s;
	spaces();
	s = (byte *)string(value, 0);
	printf("#include \"../src/%s.h\"\n", s);
	spaces();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var func() {
	byte *s;
	spaces();
	nb_ref = 0;
	var start, end;
	var i;
	var j;
	if (class_name[0] && nb_methods == 0) {
		printf("};\n");
	}

	s = (byte *)identifier(func_name);
	spaces();
	if (class_name[0]) {
		printf("var %s__%s()", class_name, s);
		if (ahead == ';') {
			next();
			func_name[0] = '\0';
			nb_methods = -1;
			printf(";\n");
			return 0;
		}
		printf(" {\n\tstruct %s *self = (void*)pop();\n", class_name);
	} else {
		printf("var %s()", s);
		if (ahead == ';') {
			next();
			func_name[0] = '\0';
			nb_methods = -1;
			printf(";\n");
			return 0;
		}
		printf(" {\n");
	}
	while (ahead != EOF && ahead != ':' && ahead != ';') {
		s = (byte *)identifiera(var_name, &start, &end);
		spaces();
		if (end < 0) {
			printf("\tvar %s = pop();\n", s);
		} else {
			printf("\tvar %s[%ld];\n", s, end + 1);
			for (i = start; i <= end; i++) {
				printf("\t%s[%ld] = pop();\n", s, i);
			}
		}
		if (ahead == ',') {
			next();
			spaces();
		} else {
			break;
		}
	}
	nb_ret = 0;
	if (ahead == ';') {
		next();
		spaces();
		while (ahead != EOF && ahead != ':' && ahead != ';') {
			if (nb_ret >= MAX_REF) {
				error("too many return args");
			}
			s = (byte *)identifiera(ret_name[nb_ret], 
					ret_start+nb_ret, ret_end+nb_ret);
			spaces();
			if (end < 0) {
				printf("\tvar %s;\n", s);
			} else {
				printf("\tvar %s[%ld];\n", s, end + 1);
			}
			nb_ret++;
			if (ahead == ',') {
				next();
				spaces();
			} else {
				break;
			}
		}
	}
	if (class_name[0]) {
		printf("\t(void)self;\n");
	}
	if (ahead == ':') {
		if (nb_methods < 0) {
			nb_methods = 0;
		}
		next();
	} else if (ahead == ';') {
		error("syntax");
		return 0;
	} else {
		error("missing ; or :");
	}
	spaces();
	while (ahead != EOF && func_name[0]) {
		keyword();
		spaces();
	}

	nb_methods++;
	for (j = 0; j < nb_ret; j++) {
		end = ret_end[j];
		start = ret_start[j];
		s = ret_name[j];
		if (end < 0) {
			printf("\tpush(%s);\n", s);
		} else {
			for (i = start; i <= end; i++) {
				printf("\tpush(%s[%ld]));\n", s, i);
			}
		}
	}
	printf("\treturn 0;\n");
	printf("}\n");

	return 0;
}

var sizeof_() {
	byte *s;
	spaces();
	s = (byte *)identifier(var_name);
	spaces();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tpush(sizeof(struct %s));\n", s);
	return 0;
}

var getb() {
	byte *s;
	spaces();
	s = (byte *)identifier(var_name);
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tpush((%s >> pop()) & 1);\n", s);
	return 0;
}

var setb() {
	byte *s;
	spaces();
	s = (byte *)identifier(var_name);
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\t%s = %s | (1 << pop());\n", s, s);
	return 0;
}

var clrb() {
	byte *s;
	spaces();
	s = (byte *)identifier(var_name);
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\t%s = %s & ~(1 << pop());\n", s, s);
	return 0;
}

var gett() {
	spaces();
	args();
	next();
	spaces();
	tabs();
	printf("\tpop();pop();push(0);/*FIXME*/\n");
	return 0;
}

var minust() {
	spaces();
	args();
	next();
	spaces();
	return 0;
}

var plust() {
	spaces();
	args();
	next();
	spaces();
	return 0;
}

var zerot() {
	spaces();
	args();
	next();
	spaces();
	return 0;
}

var get() {
	var start, end;
	byte *s;
	var i;
	spaces();

	s = (byte *)identifiera(var_name, &start, &end);
	spaces();
	if (end < 0) {
		args();
		if (ahead != ';') {
			error("missing! ;");
		}
		next();
		spaces();
		tabs();
		if (!strcmp("this", (char *)s)) {
			printf("\tpush((var)self);\n");
		} else if (is_member(s)) {
			printf("\tpush(*((var*)&self->%s));\n", s);
		} else {
			printf("\tpush(*((var*)&%s));\n", s);
		}
	} else {
		args();
		for (i = start; i<= end; i++) {
			tabs();
			if (is_member(s)) {
				printf("\tpush(*((var*)&self->%s[%ld]));\n", s, i);
			} else {
				printf("\tpush(*((var*)&%s[%ld]));\n", s, i);
			}
		}
		if (ahead != ';') {
			error("missing! ;");
		}
		next();
		spaces();
	}
	return 0;
}

var op(var operation) {
	spaces();
	args();
	tabs();
	printf("\t{var tmp = pop();push(tmp %s pop());}\n", (byte *)operation);
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}
var varadd() {
	spaces();
	args();
	tabs();
	printf("\t{var tmp = pop();push(tmp + (%ld * pop()));}\n", SIZE_OF_VAR);
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var tee(var dotee) {
	var start, end;
	byte *s;
	var i;
	spaces();

	s = (byte *)identifiera(var_name, &start, &end);
	spaces();
	if (end < 0) {
		args();
		if (ahead != ';') {
			error("missing! ;");
		}
		next();
		spaces();
		tabs();
		if (!strcmp("this", (char *)s)) {
			printf("\tself = (void*)pop();\n");
			if (dotee) {
				tabs();
				printf("\tpush((var)self);\n");
			}
		} else if (is_member(s)) {
			printf("\t*((var*)&self->%s) = pop();\n", s);
			if (dotee) {
				tabs();
				printf("\tpush(*((var*)&self->%s));\n", s);
			}
		} else {
			printf("\t*((var*)&%s) = pop();\n", s);
			if (dotee) {
				tabs();
				printf("\tpush(*((var*)&%s));\n", s);
			}
		}
	} else {
		args();
		for (i = start; i<= end; i++) {
			tabs();
			if (is_member(s)) {
				printf("\t*((var*)&self->%s[%ld]) = pop();\n", s, i);
			} else {
				printf("\t*((var*)&%s[%ld]) = pop();\n", s, i);
			}
		}
		if (ahead != ';') {
			error("missing! ;");
		}
		next();
		spaces();
	}
	return 0;
}

var set() {
	return tee(0);
}

var body() {
	printf(" {\n");
	indent++;
	spaces();
	if (ahead != '(') {
		error("missing (");
	}
	next();
	spaces();
	while ((ahead >= 'a' && ahead <= 'z') || ahead == '"' ||
	       ahead == '\'' || (ahead >= '0' && ahead <= '9') ||
	       ahead == '-') {
		keyword();
		spaces();
	}
	if (ahead != ')') {
		error("missing )");
	}
	next();
	spaces();
	indent--;
	tabs();
	printf("\t}");
	return 0;
}

var loop() {
	spaces();
	tabs();
	printf("\twhile (1)");
	body();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	printf("\n");
	return 0;
}

var if_() {
	var v;
	var n = 0;
	char *s;
	spaces();
	tabs();
	printf("\t{var tmp = pop();\n");
	while (ahead != ';' && ahead != EOF) {
		if (n > 0) {
			printf(" else ");
		} else {
			tabs();
			printf("\t");
		}
		if (ahead >= 'a' && ahead <= 'z') {
			s = (char *)identifier(var_name);
			spaces();
			if (!strcmp(s, "true")) {
				printf("if (tmp)");
			} else if (!strcmp(s, "false") || !strcmp(s, "zero")) {
				printf("if (!tmp)");
			} else if (!strcmp(s, "default") && n > 0) {
				body();
				n++;
				break;
			} else {
				error("syntax");
			}
		} else {
			v = number();
			printf("if (tmp == %ld)", v);
		}
		body();
		n++;
	}
	if (ahead != ';') {
		error("missing ;");
	}
	printf("}\n");
	next();
	spaces();
	return 0;
}

var load(char *type) {
	byte *s = NULL;
	byte *v;
	spaces();
	if (ahead != ';') {
		s = (byte *)identifier(var_name);
		spaces();
	}
	if (ahead == '.') {
		next();
		v = (byte *)identifier(value);
		spaces();
		args();
		tabs();
		if (!strcmp((char *)s, "this")) {
			printf("\tpush(*((%s*)&self->", type);
		} else {
			printf("\tpush(*((%s*)&%s->", type, s);
		}
		printf("%s));\n", v);
	} else if (s) {
		args();
		tabs();
		printf("\tpush(*((%s*)&%s", type, s);
		printf("));\n");
	} else {
		tabs();
		printf("\tpush(*((%s*)pop()));\n", type);
	}

	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var store(char *type) {
	byte *s = NULL;
	byte *v;
	spaces();
	if (ahead != ';') {
		s = (byte *)identifier(var_name);
		spaces();
	}
	if (ahead == '.') {
		next();
		v = (byte *)identifier(value);
		spaces();
		args();
		tabs();
		if (!strcmp((char *)s, "this")) {
			printf("\t*((%s*)&self->", type);
		} else {
			printf("\t*((%s*)&%s->", type, s);
		}
		printf("%s", v);
		printf(") = pop();\n");
	} else if (s) {
		args();
		tabs();
		printf("\t*((%s*)&%s", type, s);
		printf(") = pop();\n");
	} else {
		tabs();
		printf("\t{vat tmp = pop(); *((%s*)tmp);", type);
		printf(") = pop();}\n");
	}

	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var is_member(byte *id)
{
	var i;
	if (!class_name[0]) {
		return 0;
	}
	for (i = 0; i < nb_class_new; i++) {
		if (!strcmp((char*)id, (char*)class_new[i])) {
			return 1;
		}
	}
	for (i = 0; i < nb_class_var; i++) {
		if (!strcmp((char*)id, (char*)class_var[i])) {
			return 1;
		}
	}
	return 0;
}

var args() {
	byte *s = NULL;
	var n = 0;
	var i;
	var start,end;
	byte *id = NULL;
	byte *id2 = NULL;
	if (ahead == ';') {
		return 0;
	}
	if (ahead >= 'a' && ahead <= 'z') {
		id = malloc(MAX_ID_LEN);
		identifiera(id, &start, &end);
		spaces();
		if (ahead == '.') {
			error("unexpected .");
			next();
			spaces();
			id2 = malloc(MAX_ID_LEN);
			identifier(id2);
		}
	} else if (ahead == '"' || ahead == '\'') {
		var o;
		s = malloc(MAX_SIZE);
		s[0] = 0;
		while (ahead == '"' || ahead == '\'') {
			o = strlen((char *)s);
			if (ahead == '"') {
				string(s, o);
			} else {
				byte_string(s, o);
			}
			spaces();
		}
	} else if (ahead == '-') {
		n = number();
	} else if (ahead >= '0' && ahead <= '9') {
		n = number();
	} else {
		error("syntax");
	}
	spaces();
	args();
	if (id) {
		if (id2) {
			tabs();
			if (!strcmp("this", (char *)id)) {
				printf("\tpush((var)self->%s);\n", id2);
			} else {
				printf("\tpush((var)%s->%s);\n", id, id2);
			}
			free(id2);
		} else {
			tabs();
			if (!strcmp("this", (char *)id)) {
				printf("\tpush((var)self);\n");
			} else if (is_member(id)) {
				if (end < 0) {
					if (get_class_name(id)) {
						printf("\tpush((var)&self->%s);\n", id);
					} else {
						printf("\tpush((var)self->%s);\n", id);
					}
				} else {
					for (i = start; i <= end; i++) {
						printf("\tpush((var)self->%s[%ld]);\n", id, i);
					}
				}
			} else {
				if (end < 0) {
					printf("\tpush((var)%s);\n", id);
				} else {
					for (i = start; i <= end; i++) {
						printf("\tpush((var)%s[%ld]);\n", id, i);
					}
				}
			}
		}
		free(id);
	} else if (s) {
		tabs();
		printf("\tpush((var)\"%s\");\n", s);
		free(s);
	} else {
		tabs();
		printf("\tpush(%ld);\n", n);
	}
	return 0;
}

var delete_() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tfree((void*)pop());\n");
	return 0;
}

var print() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tprintf(\"%%s\",(char*)pop());\n");
	return 0;
}

var println() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tprintf(\"%%s\\n\",(char*)pop());\n");
	return 0;
}

var continue_() {
	spaces();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tcontinue;\n");
	return 0;
}

var break_() {
	spaces();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tbreak;\n");
	return 0;
}

var posedge() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\t{var tmp = pop(); push(tmp&1 && !(tmp&0x100));}\n");
	return 0;
}


var printv() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\tprintf(\"%%ld\",pop());\n");
	return 0;
}

var exit_() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\texit(pop());\n");
	return 0;
}

var const_() {
	spaces();
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var new_() {
	byte *s;
	spaces();
	if (class_name[0] && nb_methods < 1) {
		if (nb_class_new >= MAX_REF) {
			error("too many class fields");
		}
		s = (byte *)identifier(class_new_class[nb_class_new]);
		printf("\tstruct %s ", s);
		spaces();
		s = (byte *)identifier(class_new[nb_class_new]);
		nb_class_new++;
		printf("%s;\n", s);
		spaces();
		if (ahead != ';') {
			error("missing ;");
		}
		next();
		spaces();
		return 0;
	}
	args();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	printf("\t{var tmp = (var)malloc(pop());push(tmp);}\n");
	return 0;
}

var end() {
	byte *s;
	spaces();
	if (func_name[0]) {
		if (ahead != ';') {
			error("missing ;");
		}
		func_name[0] = 0;
	} else if (class_name[0]) {
		s = (byte *)identifier(var_name);
		spaces();
		if (ahead != ';' || strcmp("class", (char *)s)) {
			error("syntax");
		}
		class_name[0] = 0;
	}
	next();
	spaces();
	return 0;
}

var var_() {
	byte *s;
	var start, end;
	spaces();
	if (class_name[0] && nb_methods < 1) {
		if (nb_class_var >= MAX_REF) {
			error("too may fields");
		}
		s = (byte *)identifiera(class_var[nb_class_var], &start, &end);
		nb_class_var++;
	} else {
		s = (byte *)identifiera(var_name, &start, &end);
	}
	spaces();
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	tabs();
	if (end < 0) {
		printf("\tvar %s;\n", s);
	} else {
		printf("\tvar %s[%ld];\n", s, end + 1);
	}
	return 0;
}

var data() {
	var size = 0;
	byte *s;
	var v;
	var c;
	spaces();
	s = (byte *)identifier(var_name);
	spaces();
	printf("const byte %s[] = \"", s);
	while (ahead == '"' || ahead == '\'') {
		if (ahead == '"') {
			next();
			while ((c = next()) != EOF) {
				if (c == '"') {
					spaces();
					if (ahead != '"' && ahead != '\'') {
						printf("\\x00");
						size++;
					}
					break;
				}
				printf("%c", (int)c);
				size++;
			}
			spaces();
		} else {
			next();
			while ((c = next()) != EOF && ahead != EOF) {
				if (c == '\'') {
					break;
				}
				printf("\\x");
				if (c == '-') {
					printf("0");
				} else {
					printf("%1lX", c - 'a' + 1);
				}
				c = next();
				if (c == '\'') {
					break;
				}
				if (c == '-') {
					printf("0");
				} else {
					printf("%1lX", c - 'a' + 1);
				}
				size++;
			}
			spaces();
		}
	}

	while (ahead != EOF && ahead != ';') {
		v = number();
		spaces();
		printf("\\x%02lx", v & 0xFF);
		printf("\\x%02lx", (v >> 8) & 0xFF);
		printf("\\x%02lx", (v >> 16) & 0xFF);
		if (ahead == ',') {
			printf("\\x%02lx", (v >> 24) & 0xFF);
			size += 4;
			next();
			spaces();
		} else {
			printf("\\x%02lx", (v >> 24) & 0xFF);
			size += 4;
			break;
		}
	}
	if (ahead == ';') {
		printf("\"; /* size : %ld */\n", size);
		next();
		return 0;
	} else {
		error("missing ;");
	}
	return 0;
}

var class_() {
	byte *s;
	if (func_name[0]) {
		if (nb_ref >= MAX_REF) {
			error("too many object ref");
		}
		spaces();
		s = (byte *)identifier(ref_class[nb_ref]);
		printf("\tstruct %s *", s);
		spaces();
		s = (byte *)identifier(ref_name[nb_ref]);
		printf("%s = NULL;\n", s);
		spaces();
		if (ahead != ';') {
			error("miss ;");
		}
		next();
		spaces();
		nb_ref++;
		return 0;
	}
	if (class_name[0]) {
		error("class in class");
	}
	nb_class_new = 0;
	nb_class_var = 0;
	spaces();
	s = (byte *)identifier(class_name);
	spaces();
	if (ahead != ':') {
		error("missing :");
	}
	next();
	spaces();
	nb_methods = 0;
	printf("struct %s {\n", class_name);
	while (class_name[0] && ahead >= 'a' && ahead <= 'z') {
		keyword();
		spaces();
	}
	return 0;
}

var get_class_name(byte *first) {
	var i;
	for (i = 0; i < nb_ref; i++) {
		if (!strcmp((char *)first, (char *)ref_name[i])) {
			return (var)ref_class[i];
		}
	}
	if (class_name[0]) {
		for (i = 0; i < nb_class_new; i++) {
			if (!strcmp((char *)first, (char *)class_new[i])) {
				return (var)class_new_class[i];
			}
		}
	}
	return 0;
}

var drop() {
	spaces();
	if (ahead != ';') {
		keyword();
	} else {
		next();
	}
	tabs();
	printf("\tpop();\n");
	spaces();
	return 0;
}

var call(byte *first) {
	byte *s;
	spaces();
	if (ahead == '.') {
		next();
		spaces();
		s = (byte *)identifier(var_name);
		spaces();
		args();
		if (!strcmp((char *)first, "this")) {
			tabs();
			printf("\tpush((var)self);\n");
			tabs();
			printf("\t((var(*)(void))%s__%s)();\n", class_name, s);
		} else {
			tabs();
			if (is_member(first)) {
				printf("\tpush((var)&self->%s);\n", first);
			} else {
				printf("\tpush((var)%s);\n", first);
			}
			tabs();
			printf("\t((var(*)(void))%s__%s)();\n",
			       (char *)get_class_name(first), s);
		}
	} else {
		args();
		tabs();
		printf("\t((var(*)(void))%s)();\n", first);
	}
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var fill() {
	args();
	tabs();
	printf("\t{var offset = pop(); var val = pop(); var size = pop();");
	printf("memset(offset, val, size);}\n");
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var copy() {
	args();
	printf("{var dest = pop(); var src = pop(); var size = pop();");
	printf("memmove(dest, src, size);}\n");
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var init() {
	args();
	printf("{var seg = pop(); var dest = pop(); var src = pop(); var size "
	       "= pop();");
	printf("memcpy(dest, src + seg, size);}\n");
	if (ahead != ';') {
		error("missing ;");
	}
	next();
	spaces();
	return 0;
}

var keyword() {
	var i;
	tmp[0] = 0;
	switch (ahead) {
	case 'a':
		if (!kcmp("add", tmp)) {
			return op((var) "+");
		}
		break;
	case 'b':
		if (!kcmp("break", tmp)) {
			return break_();
		}
		break;
	case 'c':
		if (!kcmp("class", tmp)) {
			return class_();
		} else if (!kcmp("copy", tmp)) {
			return copy();
		} else if (!kcmp("const", tmp)) {
			return const_();
		} else if (!kcmp("clrb", tmp)) {
			return clrb();
		} else if (!kcmp("continue", tmp)) {
			return continue_();
		}
		break;
	case 'd':
		if (!kcmp("delete", tmp)) {
			return delete_();
		} else if (!kcmp("drop", tmp)) {
			return drop();
		} else if (!kcmp("div", tmp)) {
			return op((var) "/");
		} else if (!kcmp("data", tmp)) {
			return data();
		}
		break;
	case 'e':
		if (!kcmp("end", tmp)) {
			return end();
		} else if (!kcmp("eq", tmp)) {
			return op((var) "==");
		} else if (!kcmp("exit", tmp)) {
			return exit_();
		}
		break;
	case 'f':
		if (!kcmp("func", tmp)) {
			return func();
		} else if (!kcmp("fill", tmp)) {
			return fill();
		}
		break;
	case 'g':
		if (!kcmp("get", tmp)) {
			return get();
		} else if (!kcmp("getb", tmp)) {
			return getb();
		} else if (!kcmp("gett", tmp)) {
			return gett();
		} else if (!kcmp("gt", tmp)) {
			return op((var) ">");
		} else if (!kcmp("ge", tmp)) {
			return op((var) ">=");
		}
		break;
	case 'i':
		if (!kcmp("include", tmp)) {
			return include();
		} else if (!kcmp("init", tmp)) {
			return init();
		} else if (!kcmp("if", tmp)) {
			return if_();
		}
		break;
	case 'l':
		if (!kcmp("load", tmp)) {
			return load("var");
		} else if (!kcmp("loadb", tmp)) {
			return load("byte");
		} else if (!kcmp("loop", tmp)) {
			return loop();
		} else if (!kcmp("lt", tmp)) {
			return op((var) "<");
		} else if (!kcmp("le", tmp)) {
			return op((var) "<=");
		}
		break;
	case 'm':
		if (!kcmp("mul", tmp)) {
			return op((var) "*");
		} else if (!kcmp("minust", tmp)) {
			return minust();
		}
		break;
	case 'n':
		if (!kcmp("new", tmp)) {
			return new_();
		} else if (!kcmp("ne", tmp)) {
			return op((var) "!=");
		}
		break;
	case 'p':
		if (!kcmp("plust", tmp)) {
			return plust();
		} else if (!kcmp("print", tmp)) {
			return print();
		} else if (!kcmp("posedge", tmp)) {
			return posedge();
		} else if (!kcmp("println", tmp)) {
			return println();
		} else if (!kcmp("printv", tmp)) {
			return printv();
		}
		break;
	case 'r':
		if (!kcmp("rem", tmp)) {
			return op((var) "%");
		}
		break;
	case 's':
		if (!kcmp("set", tmp)) {
			return set();
		} else if (!kcmp("sub", tmp)) {
			return op((var) "-");
		} else if (!kcmp("setb", tmp)) {
			return setb();
		} else if (!kcmp("store", tmp)) {
			return store("var");
		} else if (!kcmp("storeb", tmp)) {
			return store("byte");
		} else if (!kcmp("sizeof", tmp)) {
			return sizeof_();
		}
		break;
	case 't':
		if (!kcmp("tee", tmp)) {
			return tee(1);
		}
		break;
	case 'v':
		if (!kcmp("var", tmp)) {
			return var_();
		} else if (!kcmp("varadd", tmp)) {
			return varadd();
		}
		break;
	case 'w':
		if (!kcmp("wire", tmp)) {
			return var_();
		}
		break;
	case 'z':
		if (!kcmp("zerot", tmp)) {
			return zerot();
		}
		break;
	default:
		if (ahead < 'a' || ahead > 'z') {
			if ((ahead >= '0' && ahead <= '9') || ahead == '-') {
				return const_();
			} else if (ahead == '"' || ahead == '\'') {
				return const_();
			}
			error("syntax");
			return -1;
		}
	}
	i = 0;
	while (tmp[i]) {
		i++;
	}
	while (ahead != EOF && i < MAX_ID_LEN) {
		if (ahead >= 'a' && ahead <= 'z') {
			tmp[i] = next();
			i++;
		} else {
			break;
		}
	}
	tmp[i] = 0;
	if (i > 0) {
		return call(tmp);
	}
	return -1;
}

var comment() {
	while (ahead != EOF) {
		switch (ahead) {
		case '\n':
			line++;
			next();
			return 0;
		}
		next();
	}
	return -1;
}

var process(var argc, byte **argv) {
	line = 1;
	var_name[0] = 0;
	func_name[0] = 0;
	class_name[0] = 0;
	next();
	while (ahead != EOF) {
		switch (ahead) {
		case '!':
			comment();
			spaces();
			break;
		case '\n':
			line++;
		case '\r':
		case ' ':
		case '\t':
			next();
			break;
		default:
			if (ahead >= 'a' && ahead <= 'z') {
				keyword();
			} else {
				error("unexpected");
			}
		}
	}
	printf("var stack[MAX_STACK + 1];\n");
	printf("var sp = 0;\n");
	printf("int main(int argc, char *argv[]) {\n");
	printf("\tpush((var)argv);\n");
	printf("\tpush((var)argc);\n");
	printf("\tstart();\n");
	printf("\treturn (int)pop();\n}\n");

	return 0;
}

int main(int argc, char *argv[]) {
	/* call the main...*/
	return (int)process(argc, (byte **)argv);
}
