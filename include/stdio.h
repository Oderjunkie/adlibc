#include <sys/types.h>
#include "stdarg.h"

enum { EOF = -1 };
struct FILE;
typedef struct FILE FILE;
typedef long int fpos_t;

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
extern int fprintf(FILE *, const char *, ...);
extern int printf(const char *, ...);
extern int sprintf(char *, const char *, ...);
extern int vfprintf(FILE *, const char *, va_list);
extern int vprintf(const char *, va_list);
extern int vsprintf(char *, const char *, va_list);
extern char *fgets(char *, int, FILE *);
extern long int ftell(FILE *);
extern int fseek(FILE *, long int, int);
extern int fgetpos(FILE *, fpos_t *);
extern int fsetpos(FILE *, const fpos_t *);
extern void rewind(FILE *);
extern void clearerr(FILE *);
extern int ungetc(int, FILE *);
extern char *gets(char *);

