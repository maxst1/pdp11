#include "run.h"
#include "pdp11.h"
#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"

byte b, r;
word nn;
Arg ss, dd;
byte N, Z, C;
char xx;
char * output_text;

void sval(word* resval, word resadr, byte b) {
	if (b == 0) 
		*resval = w_read(resadr);
	else 
		*resval = ((b_read(resadr) >> 7) * 0177400) | b_read(resadr);
			
}

void print_flags() {
	trace("N = %d\nZ = %d\nC = %d\n", N, Z, C);
}

void print_regs() {
	for (int i = 0; i < 8; i++)
		trace("R%o=%06o\n", i, reg[i]);
	trace("\n");
}

Arg get_ds(word w) {
	Arg res;
	int r = w & 7;
	int mode = (w >> 3) & 7;
	
	switch(mode) {
		case 0:
			res.type = 0;
			res.adr = r;
			res.val	= reg[r];
			trace("R%o ", r);
			break;
		case 1:
			res.type = 1;
			res.adr = reg[r];
			sval(&(res.val), res.adr, b);
			trace("(R%o) ", r);
			break;
		case 2:
			res.type = 1;
			if (r == 7) {
				res.adr = reg[r];
				res.val = w_read(res.adr);
				reg[r] += 2;
				trace("#%06o ", res.val);
			} else if (r == 6) {
				res.adr = reg[r];
				res.val = w_read(res.adr);
				reg[r] += 2;
				trace("(SP)+ ");
			} else {
				res.adr = reg[r];
				if (b == 0) {
					res.val = w_read(res.adr);
					reg[r] += 2;
				} else {
					res.val = ((b_read(res.adr) >> 7) * 0177400) | b_read(res.adr);
					reg[r] += 1;
				}
				trace("(R%o)+ ", r);
			}
			break;
		case 3:
			res.type = 1;
			res.adr = w_read(reg[r]);
			sval(&(res.val), res.adr, b);
			reg[r] += 2;
			if (r == 7)
				trace("@#%o", res.adr);
			else
				trace("@(R%o)+ ", r);
			break;
		case 4:
			res.type = 1;
			if (b == 0) {
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read(reg[r]);
			} else if (r == 6) {
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read(res.adr);
				trace("-(SP)");
			} else {
				reg[r] -= 1;
				res.adr = reg[r];
				res.val = ((b_read(res.adr) >> 7) * 0177400) | b_read(res.adr);
			}
			trace("-(R%o) ", r);
			break;
		case 6:
			res.type = 1;
			pc += 2;
			res.adr = reg[r] + w_read(pc - 2);
			sval(&(res.val), res.adr, b);
			if (r == 7)
				trace("%06o", res.adr);
			else
				trace("%o(R%o)", w_read(pc - 2), r);
			break;
		case 7:
			res.type = 1;
			pc += 2;
			res.adr = reg[r] + w_read(pc - 2);
			res.adr = w_read(res.adr);
			sval(&(res.val), res.adr, b);
			if (r == 7)
				trace("@%06o", reg[r] + w_read(pc - 2));
			else
				trace("@%o(R%o)", w_read(pc - 2), r);
			break;
		default:
			trace("unknown mode");
			exit(0);
			break;
	}

	return res;
}

Command cmd[] = {
	{0070000, 0010000, "MOVb", mov},
	{0170000, 0060000, "ADD", add},
	{0177777, 0000000, "HALT", halt},
	{0177000, 0077000, "SOB", SOB},
	{0177700, 0005000, "CLRb", CLR},
	{0077700, 0005700, "TSTb", tst},
	{0070000, 0020000, "CMPb", cmp},
	{0177740, 0000240, "CS", cs},
	{0177400, 0000400, "BR", br},
	{0177400, 0001400, "BEQ", beq},
	{0177400, 0100000, "BPL", bpl},
	{0177000, 0004000, "JSR", jsr},
	{0177770, 0000200, "RTS", rts},
	{0177777, 0177777, "UNKNOWN", unknown}
};

void run() {
	pc = 01000;
	w_write(ostat, 0177777);
	while (1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w);
		pc += 2;
		int i = 0;
		while (1) {
			if ((w & cmd[i].mask) == cmd[i].opcode) {
				trace("%s ", cmd[i].name);
				cmd[i].do_func(w);
				break;
			} 
			i++;
			if ( (cmd[i].mask == 0177777) && (cmd[i].opcode == 0177777) ) {
				trace("%s ", cmd[i].name);
				cmd[i].do_func(w);
				break;
			}
		}
		if (trac == 2) {
			print_regs();
			print_flags();
		}	
	}
}