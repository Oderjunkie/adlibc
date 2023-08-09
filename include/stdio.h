#include <sys/types.h>

enum { EOF = -1 };
struct FILE;
typedef struct FILE FILE;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
extern FILE *fopen(const char *, const char *);
extern int fclose(FILE *);
extern size_t fwrite(const void *, size_t, size_t, FILE *);
extern size_t fread(void *, size_t, size_t, FILE *);
extern int fgetc(FILE *);
extern int getc(FILE *);
extern int getchar(void);
extern int fputc(int, FILE *);
extern int putc(int, FILE *);
extern int putchar(int);
extern int feof(FILE *);
extern int ferror(FILE *);
extern int fputs(const char *, FILE *);
extern int puts(const char *);

