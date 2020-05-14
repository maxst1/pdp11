#include "pdp11.h"
#include "run.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Использование: ./(...) [-t -T] <path>\n");
        return 0;
    }
	test();
	if (!strcmp(argv[1], "-t")) {
		trac = 1;
		load_file(argv[2]);
	} else if (!strcmp(argv[1], "-T")) {
		trac = 2;
		load_file(argv[2]);
	} else {
		load_file(argv[1]);
	}
	run();
    return 0;
}
