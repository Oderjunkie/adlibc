#include "include/stdlib.h"
#include "include/string.h"
#include "include/ctype.h"
#include "syscalls.h"

extern void *malloc(size_t len) {
  void *out;
  out = __mmap(
    (void *) 0, /* do not place it at a specific address */
    sizeof(size_t) + len, /* allocate enough space for the length of the buffer and the buffer itself */
    PROT_READ | PROT_WRITE, /* read-write permissions */
    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 /* don't read this memory from a file */
  );
  *(size_t *) out = len;
  return (void *) ((size_t *) out + 1);
}

extern void free(void *ptr) {
  size_t *len;

  len = ((size_t *) ptr - 1);
  __munmap(len, *len);
}

extern void *realloc(void *ptr, size_t newlen) {
  if (ptr == (void *) 0) {
    return malloc(newlen);
  } else if (newlen == 0) {
    free(ptr);
    return 0;
  } else {
    size_t *oldlen;

    oldlen = ((size_t *) ptr - 1);
    if (newlen <= *oldlen) {
      return ptr;
    } else {
      void *newptr;

      newptr = malloc(newlen);
      memcpy(newptr, ptr, *oldlen);
      free(ptr);
      return newptr;
    }
  }
}

const char digits[] = "0123456789";

extern int atoi(const char *str) {
  int magnitude = 0;
  int sign = 1;

  while (isspace(*str))
    ++str;

  if (*str == '+') {
    sign = 1;
    ++str;
  } else if (*str == '-') {
    sign = -1;
    ++str;
  }

  while (isdigit(*str)) {
    magnitude *= 10;
    magnitude += strchr(digits, *str) - digits;
    ++str;
  }

  return magnitude * sign;
}

#define EXIT_HANDLER_MAX 32
static void (*exit_handlerv[EXIT_HANDLER_MAX])(void) = { 0 };
static int exit_handlerc = 0;
extern void exit(int code) {
  int i;

  for (i = exit_handlerc - 1; i >= 0; --i)
    (*exit_handlerv[i])();
  (void) __exit(code);

#ifdef __GNUC__
  __builtin_unreachable();
#endif
}

extern int atexit(void (*handler)(void)) {
  if (exit_handlerc < EXIT_HANDLER_MAX) {
    exit_handlerv[exit_handlerc++] = handler;
    return 0;
  } else {
    return -1;
  }
}

extern int abs(int x) {
  return (int) labs((long int) x);
}

extern long int labs(long int x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

