#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/string.h"
#include "include/stdarg.h"
#include "include/ctype.h"
#include "include/limits.h"

enum { UNSPECIFIED = -1 };

struct flags_printf {
  unsigned int leftjustify         : 1;
  unsigned int alwaysdisplaysign   : 1;
  unsigned int displayplusasspace  : 1;
  unsigned int alternate           : 1;
  unsigned int leadingzeros        : 1;
};

enum size_printf {
  H     = -1,
  BLANK = 0,
  L     = 1,
  CAPSL = 6
};

enum conversionstyle_printf {
  CHARACTER                      = 'c',
  STRING                         = 's',
  SIGNED_INT                     = 'd',
  OCTAL_UNSIGNED_INT             = 'o',
  LOW_HEXADECIMAL_UNSIGNED_INT   = 'x',
  CAPS_HEXADECIMAL_UNSIGNED_INT  = 'X',
  UNSIGNED_INT                   = 'u',
  LOW_FLOAT                      = 'f',
  CAPS_FLOAT                     = 'F',
  LOW_EXPONENTIAL_FLOAT          = 'a',
  CAPS_EXPONENTIAL_FLOAT         = 'A',
  LOW_SMART_FLOAT                = 'g',
  CAPS_SMART_FLOAT               = 'G',
  NUMBER_OF_CHARS_WRITTEN        = 'n',
  POINTER                        = 'p',
  ERROR                          = '\0'
};

struct format_printf {
  struct flags_printf flags;
  enum size_printf size;
  int minwidth;
  int minprecision;
  char *ptr;
  unsigned int len;
};

static const char digits[] = "0123456789";
static const char xdigits[] = "0123456789abcdef";
static const char Xdigits[] = "0123456789ABCDEF";

static void parseflags(struct flags_printf *flags, const char **format) {
  while (strchr("-+ #0", **format))
    switch (**format) {
    case '-': flags->leftjustify = 1;        ++*format; break;
    case '+': flags->alwaysdisplaysign = 1;  ++*format; break;
    case ' ': flags->displayplusasspace = 1; ++*format; break;
    case '#': flags->alternate = 1;          ++*format; break;
    case '0': flags->leadingzeros = 1;       ++*format; break;
    }

  if (flags->alwaysdisplaysign && flags->displayplusasspace)
    flags->displayplusasspace = 0;
}

static void parsefield(int *field, const char **format, va_list args) {
  *field = 0;
  if (**format == '*') {
    *field = va_arg(args, int);
  } else while (isdigit(**format)) {
    *field *= 10;
    *field += strchr(digits, **format) - digits;
    ++*format;
  }
}

static void setdefault(int *field, int defaultval) {
  if (*field == UNSPECIFIED)
    *field = defaultval;
}

static void justify(FILE *f, int *written, struct format_printf *fmt) {
  int i;
  
  i = fmt->len;
  i = i > fmt->minwidth ? 0 : fmt->minwidth - i;
  while (i > 0) {
    fputc(' ', f);
    --i;
    ++*written;
  }
}

static void c_vfprintf(struct format_printf *fmt, va_list args) {
  *--fmt->ptr = (unsigned char) va_arg(args, int);
}

static void s_vfprintf(struct format_printf *fmt, va_list args) {
  if (fmt->minprecision == UNSPECIFIED)
    fmt->len = 0;
  else
    fmt->len = fmt->minprecision;
  fmt->ptr = va_arg(args, char *);
}

static void d_vfprintf(struct format_printf *fmt, va_list args) {
	int positive;
	long int magnitude;

	if (
    fmt->minprecision == UNSPECIFIED &&
    fmt->flags.leadingzeros &&
    !fmt->flags.leftjustify
  ) {
		fmt->minprecision += fmt->minwidth;
		if (fmt->flags.alwaysdisplaysign || fmt->flags.displayplusasspace)
			--fmt->minprecision;
	}

	switch (fmt->size) {
	case H:
	case BLANK:
		magnitude = (long int) va_arg(args, int);
		break;
	case L:
		magnitude = va_arg(args, long int);
		break;
	}

	positive = magnitude > 0;
  magnitude = labs(magnitude);

  {
    int i;
    for (i = 0; magnitude || i < fmt->minprecision; ++i) {
		  *--fmt->ptr = digits[magnitude % 10];
		  magnitude /= 10;
	  }
  }

	if (positive) {
    if (fmt->flags.alwaysdisplaysign)
      *--fmt->ptr = '+';
    else if (fmt->flags.displayplusasspace)
      *--fmt->ptr = ' ';
  } else {
		*--fmt->ptr = '-';
  }
}

static void u_vfprintf(struct format_printf *fmt, va_list args) {
	unsigned long int magnitude;

	if (
    fmt->minprecision == UNSPECIFIED &&
    fmt->flags.leadingzeros &&
    !fmt->flags.leftjustify
  ) {
		fmt->minprecision += fmt->minwidth;
	}

	switch (fmt->size) {
	case H:
	case BLANK:
		magnitude = (unsigned long int) va_arg(args, unsigned int);
		break;
	case L:
		magnitude = va_arg(args, unsigned long int);
		break;
	}

  {
    int i;
    for (i = 0; magnitude || i < fmt->minprecision; ++i) {
		  *--fmt->ptr = digits[magnitude % 10];
		  magnitude /= 10;
	  }
  }
}

static int idi(int c) { return c; }

static void x_vfprintf(struct format_printf *fmt, enum conversionstyle_printf conv, va_list args) {
	unsigned long int magnitude;
  int (*maybeupper)(int);

  maybeupper = conv == CAPS_HEXADECIMAL_UNSIGNED_INT ? toupper : idi;

	if (
    fmt->minprecision == UNSPECIFIED &&
    fmt->flags.leadingzeros &&
    !fmt->flags.leftjustify
  ) {
		fmt->minprecision += fmt->minwidth + 1;
    if (fmt->flags.alternate)
      fmt->minprecision -= 2;
	}

	switch (fmt->size) {
	case H:
	case BLANK:
		magnitude = (unsigned long int) va_arg(args, unsigned int);
		break;
	case L:
		magnitude = va_arg(args, unsigned long int);
		break;
	}

  {
    int i;
    for (i = 0; magnitude || i < fmt->minprecision; ++i) {
		  *--fmt->ptr = maybeupper(xdigits[magnitude % 16]);
		  magnitude /= 16;
	  }
  }

  if (fmt->flags.alternate) {
    *--fmt->ptr = maybeupper('x');
    *--fmt->ptr = '0';
  }
}

extern int vfprintf(FILE *f, const char *fmtstr, va_list args) {
  int written = 0;
  while (*fmtstr) {
    if (fmtstr[0] != '%') {
      fputc(*fmtstr, f);
      ++written;
      ++fmtstr;
    } else if (fmtstr[1] == '%') {
      fputc('%', f);
      ++written;
      fmtstr += 2;
    } else {
      char buffer[256] = { 0 };
      struct format_printf fmt = {
        { 0 },
        BLANK,
        UNSPECIFIED,
        UNSPECIFIED,
        buffer + sizeof(buffer) - 1,
        (unsigned int) -1
      };
      enum conversionstyle_printf conversionstyle = ERROR;

      ++fmtstr;
      parseflags(&fmt.flags, &fmtstr);
      parsefield(&fmt.minwidth, &fmtstr, args);
      if (*fmtstr == '.') {
        ++fmtstr;
        parsefield(&fmt.minprecision, &fmtstr, args);
      }

      conversionstyle = *fmtstr;
      if (conversionstyle == 'i')
        conversionstyle = 'd';

      ++fmtstr;
      if (conversionstyle == CHARACTER) {
        c_vfprintf(&fmt, args);
      } else if (conversionstyle == STRING) {
        s_vfprintf(&fmt, args);
      } else if (conversionstyle == SIGNED_INT) {
        d_vfprintf(&fmt, args);
      } else if (conversionstyle == UNSIGNED_INT) {
        u_vfprintf(&fmt, args);
      } else if (conversionstyle == LOW_HEXADECIMAL_UNSIGNED_INT
              || conversionstyle == CAPS_HEXADECIMAL_UNSIGNED_INT) {
        x_vfprintf(&fmt, conversionstyle, args);
      } else {
        return -1;
      }

      setdefault(&fmt.minwidth, 0);
      setdefault(&fmt.minprecision, 0);

      {
        size_t len;

        len = strlen(fmt.ptr);
        if (fmt.len == 0 || len < fmt.len)
          fmt.len = len;
      }

      if (!fmt.flags.leftjustify)
      	justify(f, &written, &fmt);

      if (fwrite(fmt.ptr, 1, fmt.len, f) < fmt.len)
        return -1;

      if (fmt.flags.leftjustify)
        justify(f, &written, &fmt);
    }
  }

  return written;
}

