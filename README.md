# adlibc
an alternative libc89 implementation.

## compilation
```sh
 $ make
```

## usage
assuming that you are in the root directory of the project and have just run `make`,
```sh
 $ cc -nostdlib -Os -isystem ./include ./adlibc.o -o main ./main.c
```

## featureset
implemented features are commented, unimplemented ones are left alone:
```c
#include <assert.h>
void assert(int);

#include <ctype.h>
/* int isalnum(int); */
/* int isalpha(int); */
/* int iscntrl(int); */
/* int isdigit(int); */
/* int isgraph(int); */
/* int islower(int); */
/* int isprint(int); */
/* int ispunct(int); */
/* int isspace(int); */
/* int isupper(int); */
/* int isxdigit(int); */
/* int tolower(int); */
/* int toupper(int); */

#include <errno.h>
extern int errno;

#include <math.h>
double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan(double);
double atan2(double, double);
double sinh(double);
double cosh(double);
double tanh(double);
double exp(double);
double log(double);
double log10(double);
double pow(double, double);
double sqrt(double);
double ceil(double);
double floor(double);
double fabs(double);
double ldexp(double, int);
double frexp(double, int *);
double modf(double, double *);
double fmod(double, double);

#include <setjmp.h>
typedef ... jmp_buf[...];
int setjmp(jmp_buf);
void longjmp(jmp_buf, int);

#include <signal.h>
void (*signal(int, void (*)(int)))(int);
int raise(int);

#include <stdarg.h>
void va_start(va_list, any);
T va_arg(va_list, type T);

#include <stdio.h>
/* typedef struct { ... } FILE; */
/* FILE *stdin, *stdout, *stderr; */
/* FILE *fopen(const char *, const char *); */
FILE *freopen(const char *, const char *, FILE *);
int fflush(FILE *);
/* int fclose(FILE *); */
int remove(const char *);
int rename(const char *, const char *);
FILE *tmpfile();
char *tmpname(char[L_tmpnam]);
int setvbuf(FILE *, char *, int, size_t);
void setbuf(FILE *, char *);
int fprintf(FILE *, const char *, ...);
int printf(const char *, ...);
int sprintf(char *, const char *, ...);
int vfprintf(FILE *, const char *, va_list);
int vprintf(const char *format, va_list);
int vsprintf(char *, const char *, va_list);
int fscanf(FILE *, const char *, ...);
int scanf(const char *, ...);
int sscanf(char *, const char *, ...);
/* int fgetc(FILE *); */
/* char *fgets(char *, int, FILE *); */
/* int fputc(int, FILE *); */
/* char *fputs(const char *, FILE *); */
/* int getc(FILE *); */
/* int getchar(); */
char *gets(char *);
/* int putc(int c, FILE* stream); */
/* int putchar(int); */
/* int puts(const char *); */
/* int ungetc(int, FILE *); */
/* size_t fread(void *, size_t, size_t, FILE *); */
/* size_t fwrite(const void *, size_t, size_t, FILE *); */
/* int fseek(FILE *, long, int); */
/* long ftell(FILE *); */
/* void rewind(FILE *); */
/* int fgetpos(FILE *, fpos_t *); */
/* int fsetpos(FILE *, const fpos_t *); */
/* void clearerr(FILE *); */
/* int feof(FILE *); */
/* int ferror(FILE *); */
void perror(const char *);

#include <stdlib.h>
double atof(const char *);
int atoi(const char *);
long atol(const char *);
double strtod(const char *, char **);
long strtol(const char *, char **, int);
unsigned long strtoul(const char *, char **, int);
int rand(void);
void srand(unsigned int);
void *calloc(size_t, size_t);
/* void *malloc(size_t size); */
/* void *realloc(void *, size_t); */
/* void free(void *); */
void abort(void);
void exit(int);
int atexit(void (*)(void));
int system(const char *);
char getenv(const char *);
void bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *);
void qsort(void *, size_t, size_t, int (*)(const void *, const void));
int abs(int);
long labs(long n);
div_t div(int, int);
ldiv_t ldiv(long, long);

#include <string.h>
/* char *strcpy(char *, const char *); */
/* char *strncpy(char *, const char *, int); */
char *strcat(char *, const char *);
char *strncat(char *, const char *, int);
/* int strcmp(const char *, const char *); */
int strncmp(const char *, const char *, int);
/* char *strchr(const char *, int); */
char *strrchr(const char *, int);
/* size_t strspn(const char *, const char *); */
/* size_t strcspn(const char *, const char *); */
/* char *strpbrk(const char *, const char *); */
char *strstr(const char *, const char *);
/* size_t strlen(const char *); */
char *strerror(int);
/* char *strtok(char *, const char *); */
/* void *memcpy(void *, const void *, int); */
/* void *memmove(void *, const void *, int); */
int memcmp(const void *, const void *, int);
/* void *strchr(const char *, int, int); */

#include <time.h>
typedef ... clock_t;
#define CLOCKS_PER_SEC ...
typedef ... time_t;
struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_ymon;
  int tm_year;
  int tm_day;
  int tm_yday;
  int tm_isdst;
};
clock_t clock(void);
time_t time(time_t *);
double difftime(time_t, time_t);
time_t mktime(struct tm *);
char *asctime(const struct tm *);
char *ctime(const time_t);
struct tm *gmtime(const time_t);
struct tm *localtime(const time_t);
size_t strftime(char *, size_t, const char *, const struct tm *);
```
