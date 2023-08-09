#include <sys/types.h>

enum { EOF = -1 };
struct FILE;
typedef struct FILE FILE;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
extern FILE *fopen(const char *, const char *);
extern int fclose(const FILE *);
extern size_t fwrite(const void *, size_t, size_t, FILE *);
extern size_t fread(void *, size_t, size_t, FILE *);
extern int fgetc(FILE *);
#define getc(stream) fgetc(stream)
#define getchar() getc(stdin)
extern int fputc(int, FILE *);
#define putc(ch, stream) fputc(ch, stream)
#define putchar(ch) putc(ch, stdout)
extern int feof(FILE *);
extern int ferror(FILE *);
extern int fputs(const char *, FILE *);
extern int puts(const char *);

