#include "include/string.h"

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
  static char ogchr = '\0';
  char *out;

  if (str)
    curstr = str;
  else
    *curstr = ogchr;

  out = curstr + strspn(curstr, delims);
  curstr = out + strcspn(out, delims);

  ogchr = *curstr;
  *curstr = '\0';
  if (*out) {
    return out;
  } else {
    *curstr = ogchr;
    ogchr = '\0';
    return (char *) 0;
  }
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
