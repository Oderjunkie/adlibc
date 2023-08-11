#include "include/stdio.h"
#include "include/string.h"
#include "include/stdarg.h"
#include "include/ctype.h"

static char digits[] = "0123456789";
extern int vfprintf(FILE *f, const char *format, va_list args) {
  int written = 0;
  while (*format) {
    if (format[0] != '%') {
      fputc(*format, f);
      ++written;
      ++format;
    } else if (format[1] == '%') {
      fputc('%', f);
      ++written;
      format += 2;
    } else {
      enum { UNSPECIFIED = -1 };
      struct {
        unsigned int leftjustify : 1;
        unsigned int alwaysdisplaysign : 1;
        unsigned int displayplusasspace : 1;
        unsigned int alternate : 1;
        unsigned int leadingzeros : 1;
      } flags = { 0, 0, 0, 0, 0 };
      int minwidth = UNSPECIFIED;
      int minprecision = UNSPECIFIED;
      enum {
        /* HH = -2, */
        H = -1,
        BLANK = 0,
        L = 1,
        /* LL = 2, */
        /* Z = 3, */
        /* T = 4, */
        /* J = 5, */
        CAPSL = 6
      } size = BLANK;
      enum {
        ERROR,
        CHARACTER,
        STRING,
        SIGNED_INT,
        OCTAL_UNSIGNED_INT,
        LOW_HEXADECIMAL_UNSIGNED_INT,
        CAPS_HEXADECIMAL_UNSIGNED_INT,
        UNSIGNED_INT,
        LOW_FLOAT,
        CAPS_FLOAT,
        EXPONENTIAL_FLOAT,
        CAPS_EXPONENTIAL_FLOAT,
        LOW_EXPONENTIAL_FLOAT,
        CAPS_SMART_FLOAT,
        LOW_SMART_FLOAT,
        NUMBER_OF_CHARS_WRITTEN,
        POINTER
      } conversionstyle = ERROR;
      char buffer[256] = { 0 }, *bufp = buffer + sizeof(buffer) - 1;

      ++format;
      while (strchr("-+ #0", *format))
        switch (*format) {
        case '-': flags.leftjustify = 1; ++format; break;
        case '+': flags.alwaysdisplaysign = 1; ++format; break;
        case ' ': flags.displayplusasspace = 1; ++format; break;
        case '#': flags.alternate = 1; ++format; break;
        case '0': flags.leadingzeros = 1; ++format; break;
        }
      if (flags.alwaysdisplaysign && flags.displayplusasspace)
        flags.displayplusasspace = 0;
      if (*format == '*') {
        minwidth = va_arg(args, int);
      } else if (isdigit(*format)) {
        minwidth = 0;
        while (isdigit(*format)) {
          minwidth *= 10;
          minwidth += strchr(digits, *format) - digits;
          ++format;
        }
      }
      if (*format == '.') {
        ++format;
        if (*format == '*') {
          minprecision = va_arg(args, int);
        } else if (isdigit(*format)) {
          minprecision = 0;
          while (isdigit(*format)) {
            minprecision *= 10;
            minprecision += strchr(digits, *format) - digits;
            ++format;
          }
        }
      }
      switch (*format) {
      case 'c': conversionstyle = CHARACTER; break;
      case 's': conversionstyle = STRING; break;
      case 'd':
      case 'i': conversionstyle = SIGNED_INT; break;
      case 'o': conversionstyle = OCTAL_UNSIGNED_INT; break;
      case 'x': conversionstyle = LOW_HEXADECIMAL_UNSIGNED_INT; break;
      case 'X': conversionstyle = CAPS_HEXADECIMAL_UNSIGNED_INT; break;
      case 'u': conversionstyle = UNSIGNED_INT; break;
      case 'f': conversionstyle = LOW_FLOAT; break;
      case 'F': conversionstyle = CAPS_FLOAT; break;
      case 'a': conversionstyle = LOW_EXPONENTIAL_FLOAT; break;
      case 'A': conversionstyle = CAPS_EXPONENTIAL_FLOAT; break;
      case 'g': conversionstyle = LOW_SMART_FLOAT; break;
      case 'G': conversionstyle = CAPS_SMART_FLOAT; break;
      case 'n': conversionstyle = NUMBER_OF_CHARS_WRITTEN; break;
      case 'p': conversionstyle = POINTER; break;
      }
      ++format;
      if (conversionstyle == CHARACTER) {
        minwidth = minwidth != UNSPECIFIED ? minwidth : 0;
        minprecision = minprecision != UNSPECIFIED ? minprecision : 0;
        --bufp;
        *bufp = (unsigned char) va_arg(args, int);
      } else if (conversionstyle == SIGNED_INT) {
        long int sign = 1, magnitude = 0;
        int i;

        minwidth = minwidth != UNSPECIFIED ? minwidth : 0;
        minprecision = minprecision != UNSPECIFIED ? minprecision : 0;
        switch (size) {
        case H:
        case BLANK: magnitude = (long int) va_arg(args, int); break;
        case L: magnitude = va_arg(args, long int); break;
        }
        if (magnitude < 0) {
          sign = -1;
          magnitude = -magnitude;
        }
        for (i = 0; magnitude || i < minprecision; ++i) {
          --bufp;
          *bufp = digits[magnitude % 10];
          magnitude /= 10;
        }
        if (sign == 1 && flags.alwaysdisplaysign) {
          --bufp;
          *bufp = '+';
        } else if (sign == 1 && flags.displayplusasspace) {
          --bufp;
          *bufp = '-';
          *bufp = ' ';
        } else if (sign == -1) {
          --bufp;
          *bufp = '-';
        }
      } else {
        return -1;
      }
      if (!flags.leftjustify) {
        int i;

        i = strlen(bufp);
        i = i > minwidth ? 0 : minwidth - i;
        while (i > 0) {
          fputc(' ', f);
          --i;
        }
      }
      fputs(bufp, f);
      if (flags.leftjustify) {
        int i;

        i = strlen(bufp);
        i = i > minwidth ? 0 : minwidth - i;
        while (i > 0) {
          fputc(' ', f);
          --i;
        }
      }
    }
  }
}

