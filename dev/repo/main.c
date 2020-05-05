#include "pdp11.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("Использование: ./(...) <path>\n");
        return 0;
    }
    test();
    load_file(argv[1]);
    mem_dump(0x40, 4);
    return 0;
}
