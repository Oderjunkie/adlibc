#include "include/stdlib.h"
#include "include/string.h"
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

