#include <stdio.h>
#include <stdlib.h>
#include "pdp11.h"
#include "funcs.h"
#include "run.h"

void push(byte r) {
	trace("mov ");
	mov(0010000 + (r << 6) + 046);
}

void pop(byte r) {
	trace("mov ");
	mov(0010000 + 02600 + r);
}

void jsr(word w) {
	b = 0;
	r = (w >> 6) & 7;
	trace("R%o ", r);
	dd = get_ds(w);
	trace("\n");
	word tmp = dd.adr;
	push(r);
	reg[r] = pc;
	pc = tmp;
}

void rts(word w) {
	r = w & 7;
	trace("R%o\n", r);
	pc = reg[r];
	pop(r);
}

void bpl(word w) {
	if (N == 0)
		br(w);
	else
		trace("(FALSE)\n");
}

void beq(word w) {
	if (Z == 1)
		br(w);
	else
		trace("(FALSE)\n");
}

void br(word w) {
	xx = (char)(w & 0377);
	pc = pc + 2 * xx;
	trace("%06o\n", pc);
}

void cs(word w) {
	switch(w & 0000020) {
		case 1:
			switch(w & 0000017) {
				case 0000017:
					trace("SCC ");
					N = 1;
					Z = 1;
					C = 1;
					break;
				case 1:
					trace("SEC ");
					C = 1;
					break;
				case 4:
					trace("SEZ ");
					Z = 1;
					break;
				case 8:
					trace("SEN ");
					N = 1;
					break;
			}
			break;
		case 0:
			switch(w & 0000017) {
				case 0000017:
					trace("CCC ");
					N = 0;
					Z = 0;
					C = 0;
					break;
				case 1:
					trace("CLC ");
					C = 0;
					break;
				case 4:
					trace("CLZ ");
					Z = 0;
					break;
				case 8:
					trace("CLN ");
					N = 0;
					break;
			}
			break;
	}
	trace("\n");
}

void cmp(word w) {
	b = (w >> 15) & 1;
	ss = get_ds(w >> 6);
	dd = get_ds(w);
	Z = ((ss.val - dd.val) == 0);
	if (b == 0) {
		N = ((ss.val - dd.val) >> 15) & 1;
		C = ((ss.val + 1 + ~dd.val) >= MEM_SIZE);
	} else {
		N = (((byte)ss.val - (byte)dd.val) >> 7) & 1;
		C = (((byte)ss.val + 1 + ~((byte)dd.val)) >= 256);
	}
	trace("\n");
}

void tst(word w) {
	C = 0;
	b = (w >> 15) & 1;
	dd = get_ds(w);
	if (dd.type == 0) {
		Z = (reg[dd.adr] == 0);
		if (b == 0)
			N = ((reg[dd.adr] >> 15) & 1);
		else
			N = ((reg[dd.adr] >> 7) & 1);
	} else {
		if (b == 0) {
			Z = (w_read(dd.adr) == 0);
			N = ((w_read(dd.adr) >> 15) & 1); 
		} else {
			Z = (b_read(dd.adr) == 0);
			N = ((b_read(dd.adr) >> 7) & 1);
		}
	}
	C = 0;
	trace("\n");
}

void mov(word w) {
	b = (w >> 15) & 1;
	ss = get_ds(w >> 6);
	dd = get_ds(w);
	if  (dd.type == 0)
		reg[dd.adr] = ss.val;
	else
		if (b == 0)
			w_write(dd.adr, ss.val);
		else
			b_write(dd.adr, (byte)(ss.val & 255));
		
	C = 0;
	Z = (ss.val == 0);
	N = (ss.val >> 15) & 1;
	trace("\n");
}

void SOB(word w) {
	r = (w >> 6) & 7;
	nn = w & 63;
	if (--reg[r] != 0)
		pc = pc - 2 * nn;	
	trace("R%o %06o", r, pc);
	trace("\n");
}

void CLR(word w) {
	b = (w >> 15) & 1;
	dd = get_ds(w);
	if (dd.type = 0)
		reg[dd.adr] = 0;
	else 
		if (b == 0)
			w_write(dd.adr, 0);
		else
			b_write(dd.adr, 0);

	Z = 1;
	N = 0;
	C = 0;
	trace("\n");
}

void add(word w) {
	b = (w >> 15) & 1;
	ss = get_ds(w >> 6);
	dd = get_ds(w);
	if (dd.type == 0)
		reg[dd.adr] = dd.val + ss.val;
	else
		if (b == 0)
			w_write(dd.adr, dd.val + ss.val);
		else
			b_write(dd.adr, dd.val + ss.val);
	
	Z = (dd.val + ss.val == 0);
	N = ((dd.val + ss.val) >> 15) & 1;
	C = ((dd.val + ss.val) >= MEM_SIZE);
	trace("\n");
}

void halt(word w) {
	trace("\n\n~~~~~~~~~~~halt~~~~~~~~~~~\n");
	print_regs();
	exit(0);
}

void unknown(word w) {
	trace("\n");
}