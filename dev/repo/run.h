#include "pdp11.h"

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (*do_func)(word w);
} Command;

typedef struct {
	word val; //значение аргумента
	word adr; //адресс аргумента
	byte type;
} Arg;

extern Command cmd[];
extern Arg ss, dd;
extern word nn;
extern byte b, r, t;
extern byte N, Z, C;
extern char xx;
extern char * output_text;

Arg get_ds(word w);

void run();
void print_regs();
void print_flags();
void sval(word* resval, word resadr, byte b);