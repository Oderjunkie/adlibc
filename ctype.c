#include "include/ctype.h"
#include "include/string.h"

static const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
static const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

extern int isalnum(int ch) {
  return isdigit(ch) || isalpha(ch);
}

extern int isalpha(int ch) {
  return isupper(ch) || islower(ch);
}

extern int iscntrl(int ch) {
  return !isprint(ch);
}

extern int isdigit(int ch) {
  return strchr("0123456789", (char) ch) != 0;
}

extern int isgraph(int ch) {
  return ispunct(ch) || isalnum(ch);
}

extern int islower(int ch) {
  return strchr(lowercase, (char) ch) != 0;
}

extern int isprint(int ch) {
  return isgraph(ch) || (ch == ' ');
}

extern int ispunct(int ch) {
  return strchr("!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~", ch) != 0;
}

extern int isspace(int ch) {
  return strchr("\t\n\v\f\r ", (char) ch) != 0;
}

extern int isupper(int ch) {
  return strchr(uppercase, (char) ch) != 0;
}

extern int isxdigit(int ch) {
  return isdigit(ch) || strchr("ABCDEFabcdef", (char) ch) != 0;
}

extern int tolower(int ch) {
  if (isupper(ch)) {
    size_t index;

    index = strchr(uppercase, (char) ch) - uppercase;
    return lowercase[index];
  } else {
    return ch;
  }
}

extern int toupper(int ch) {
  if (islower(ch)) {
    size_t index;

    index = strchr(lowercase, (char) ch) - uppercase;
    return uppercase[index];
  } else {
    return ch;
  }
}
