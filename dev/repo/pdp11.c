#include "pdp11.h"
#include <stdio.h>
#include <assert.h>

byte mem[MEM_SIZE];

void b_write(Adress adr, byte b) {
    mem[adr] = b;
}

byte b_read(Adress adr) {
    return mem[adr];
}

void w_write(Adress adr, word w) {
    mem[adr] = w & 0x00FF;
    mem[adr + 1] = (w >> 8) & 0x00FF;
}

word w_read(Adress adr) {
    word res = ((word)mem[adr + 1]) << 8;
    res = res | mem[adr];
    return res;
}

void test() {
    //пишем байт читаем байт
    byte b1 = 0x0a;
    b_write(0, b1);
    byte bres = b_read(0);
    //printf("%02hhx = %02hhx\n", b1, bres);
    assert(bres == b1);

    //пишем слово читаем слово
    word w1 = 0x0a0b;
    w_write(0, w1);
    word wres = w_read(0);
    //printf("%04hx = %04hx\n", w1, wres);
    assert(wres == w1);

    //пишем 2 байта читаем слово
    b1 = 0x0a;
    byte b2 = 0x0b;
    b_write(0, b1);
    b_write(1, b2);
    wres = w_read(0);
    //printf("%02hhx%02hhx = %04hx\n", b2, b1, wres);
    assert(wres == 0x0b0a);

    //пишем слово читаем 2 байта
    w1 = 0x0b0a;
    w_write(0, w1);
    byte bres1 = b_read(0);
    byte bres2 = b_read(1);
    //printf("%02hhx = 0a\n", bres1);
    //printf("%02hhx = 0b\n", bres2);
    assert(bres1 == 0x0a);
    assert(bres2 == 0x0b);
}

void load_file(const char* filename) {
    Adress a, n;
    byte c;
    FILE* input_file;
    input_file = fopen(filename, "r");
    byte b;
    while (fscanf(input_file, "%hx %hx", &a, &n) == 2) {
	for (Adress i = 0; i < n; i++) {
	    fscanf(input_file, "%hhx", &c);
	    b = c & 0xFF;
	    b_write(a + i, b);
	}
    }
    fclose(input_file);
}

void mem_dump(Adress start, word n) {   
	for (Adress i = 0; i < n / 2; i++) {
		printf("%.6o : %.6o\n", start + 2 * i, w_read(start + 2*i));
	}
}
