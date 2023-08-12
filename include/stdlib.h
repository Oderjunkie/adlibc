#include <sys/types.h>

extern void *malloc(size_t);
extern void *realloc(void *, size_t);
extern void free(void *);
extern int atoi(const char *);
extern void exit(int);
extern int atexit(void (*)(void));
extern int abs(int);
extern long int labs(long int);

