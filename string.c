#include "include/string.h"
#include "include/stdlib.h"

extern size_t strlen(const char *str) {
  size_t out = 0;
  while (*str) {
    ++out;
    ++str;
  }
  return out;
}

extern char *strchr(const char *str, int ch) {
  while (*str) {
    if ((char) ch == *str)
      return (char *) str;
    else
      ++str;
  }
  return (char *) 0;
}

extern char *strrchr(const char *str, int ch) {
  const char *next = str;
  while ((next = strchr(str, ch)))
    str = next;
  return (char *) str;
}

extern size_t strspn(const char *str, const char *prefix) {
  size_t length = 0;
  while (str[length] && strchr(prefix, str[length]))
    ++length;
  return length;
}

extern size_t strcspn(const char *str, const char *cprefix) {
  size_t length = 0;
  while (str[length] && !strchr(cprefix, str[length]))
    ++length;
  return length;
}

extern char *strpbrk(const char *str, const char *breakset) {
  return (char *) (str + strcspn(str, breakset));
}

extern void *memset(void *ptr, int ch, size_t len) {
  unsigned char *ucptr = (unsigned char *) ptr;
  while (len) {
    *ucptr++ = (unsigned char) ch;
    --len;
  }

  return ptr;
}

extern char *strtok(char *str, const char *delims) {
  static char *curstr = (char *) 0;
  char *out;

  if (str)
    curstr = str;

  out = curstr + strspn(curstr, delims);
  curstr = out + strcspn(out, delims);

  *curstr = '\0';
  if (*out)
    return out;
  else
    return (char *) 0;
}

extern void *memcpy(void *dst, const void *src, size_t len) {
  unsigned char *ucdst = (unsigned char *) dst;
  unsigned char *ucsrc = (unsigned char *) src;
  while (len) {
    *ucdst++ = *ucsrc++;
    --len;
  }
  return dst;
}

extern void *memmove(void *dst, const void *src, size_t n) {
  void *tmp = malloc(n);
  memcpy(tmp, src, n);
  memcpy(dst, tmp, n);
  free(tmp);
  return dst;
}

extern char *strcpy(char *dst, const char *src) {
  memcpy(dst, src, strlen(src) + 1);
  return dst;
}

extern char *strncpy(char *dst, const char *src, size_t len) {
  char *olddst;

  olddst = dst;
  while (len > 0 && *src) {
    *dst++ = *src++;
    --len;
  }
  return olddst;
}

extern int strcmp(const char *lhs, const char *rhs) {
  while (*lhs && *rhs) {
    if (*lhs != *rhs) {
      return *lhs - *rhs;
    }
  }

  return 0;
}
