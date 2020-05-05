typedef unsigned char byte;
typedef unsigned short int word;
typedef word Adress;

#define MEM_SIZE (64*1024)
extern byte mem[MEM_SIZE];

void test();
void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);
void load_file(const char* filename);
void mem_dump(Adress start, word n);
