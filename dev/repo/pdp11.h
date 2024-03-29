typedef unsigned char byte;
typedef	unsigned short int word;
typedef word Adress;

#define MEM_SIZE (64*1024)
extern byte mem[MEM_SIZE];
extern word reg[8];
extern byte trac;
#define pc reg[7]
#define sp reg[6]
#define odata 0177566
#define ostat 0177564

void trace(const char * format, ...);
void test();
void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);
void load_file(const char* filename);
void mem_dump(Adress start, word n);

