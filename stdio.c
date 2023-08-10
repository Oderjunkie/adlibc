#include "include/stdio.h"
#include "include/stdlib.h"
#include "include/string.h"
#include "include/stdarg.h"
#include "syscalls.h"

struct FILE {
  union {
    struct {
      int fd;
      int last_char;
    } unix_file;
    char *buffer;
  } file;
  unsigned int eof : 1;
  unsigned int error : 1;
  unsigned int ram_file : 1;
};

static FILE _stdin = { { { 0, EOF } }, 0, 0, 0 };
static FILE _stdout = { { { 1, EOF } }, 0, 0, 0 };
static FILE _stderr = { { { 2, EOF } }, 0, 0, 0 };
FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

extern int feof(FILE *f) { return f->eof; }
extern int ferror(FILE *f) { return f->error; }
extern size_t fread(void *buffer, size_t size, size_t count, FILE *f) {
  size_t i;
  unsigned char *ucbuffer = buffer;
  int c;

  for (i = 0; i < size * count; ++i) {
    if ((c = fgetc(f)) == EOF) {
      return i / size;
    }
    *ucbuffer++ = (unsigned char) c;
  }

  return count;
  return count;
}

extern size_t fwrite(const void *buffer, size_t size, size_t count, FILE *f) {
  size_t i;
  const unsigned char *ucbuffer;

  ucbuffer = (const unsigned char *) buffer;
  for (i = 0; i < size * count; ++i) {
    if (fputc(*ucbuffer, f) == EOF) {
      return i / size;
    }
    ++ucbuffer;
  }

  return count;
}

static int fgperror(unsigned char ucch, ssize_t bytes, FILE *f) {
  if (bytes > 0) {
    f->eof = 0;
    f->error = 0;
    return ucch;
  } else if (bytes < 0) {
    f->eof = 0;
    f->error = 1;
    return EOF;
  } else {
    f->eof = 1;
    f->error = 0;
    return EOF;
  }
}

extern int fgetc(FILE *f) {
  unsigned char ucch;

  if (f->ram_file) {
    f->eof = 0;
    f->error = 0;
    return *f->file.buffer++;
  } else if (f->file.unix_file.last_char != EOF) {
    int c;

    c = f->file.unix_file.last_char;
    f->file.unix_file.last_char = EOF;
    return c;
  } else {
    ssize_t bytes;

    bytes = __read(f->file.unix_file.fd, &ucch, 1);
    return fgperror(ucch, bytes, f);
  }
}

extern int fputc(int ch, FILE *f) {
  unsigned char ucch = (unsigned char) ch;

  ucch = (unsigned char) ch;
  if (f->ram_file) {
    f->eof = 0;
    f->error = 0;
    *f->file.buffer++ = (char) ucch;
    return (int) ucch;
  } else {
    ssize_t bytes;

    bytes = __write(f->file.unix_file.fd, &ucch, 1);
    return fgperror(ucch, bytes, f);
  }
}

extern int fputs(const char *str, FILE *f) {
  size_t len;

  len = strlen(str);
  if (fwrite(str, 1, len, f) < len) {
    f->error |= f->eof;
    f->eof = 0;
    return EOF;
  } else {
    return 0;
  }
}

extern int puts(const char *str) {
  if (fputs(str, stdout) == EOF)
    return EOF;
  if (fputc('\n', stdout) == EOF) {
    stdout->error |= stdout->eof;
    stdout->eof = 0;
    return EOF;
  }
  return 0;
}

extern FILE *fopen(const char *filename, const char *mode) {
  int flags = 0;
  mode_t unixmode = 0;
  FILE *out;

  /* TODO: this does not parse "rb+" correctly. */
  switch (mode[0] ^ mode[1]) {
  case 'r': flags = O_RDONLY; break;
  case 'w': flags = O_WRONLY; unixmode = O_CREAT | O_TRUNC; break;
  case 'a': flags = O_WRONLY; unixmode = O_CREAT | O_APPEND; break;
  case 'r' ^ '+': flags = O_RDWR; break;
  case 'w' ^ '+': flags = O_RDWR; unixmode = O_CREAT | O_TRUNC; break;
  case 'a' ^ '+': flags = O_RDWR; unixmode = O_CREAT | O_APPEND; break;
  }

  if (!(out = malloc(sizeof(*out))))
    return (FILE *) 0;
  memset(out, 0, sizeof(*out));

  if ((out->file.unix_file.fd = __open(filename, flags, unixmode)) == -1) {
    free(out);
    return (FILE *) 0;
  } else {
    out->file.unix_file.last_char = EOF;
    return out;
  }
}

extern int fclose(FILE *f) {
  if (f->ram_file)
    f->file.buffer = (char *) 0;
  else
    if (__close(f->file.unix_file.fd) == -1)
      return EOF;
  return 0;
}

extern int putc(int ch, FILE *f) {
  return fputc(ch, f);
}

extern int putchar(int ch) {
  return fputc(ch, stdout);
}

extern int getc(FILE *f) {
  return fgetc(f);
}

extern int getchar(void) {
  return fgetc(stdin);
}

extern int printf(const char *format, ...) {
  va_list args;
  int out;

  va_start(args, format);
  out = vprintf(format, args);
  va_end(args);
  return out;
}

extern int fprintf(FILE *f, const char *format, ...) {
  va_list args;
  int out;

  va_start(args, format);
  out = vfprintf(f, format, args);
  va_end(args);
  return out;
}

extern int sprintf(char *buffer, const char *format, ...) {
  va_list args;
  int out;

  va_start(args, format);
  out = vsprintf(buffer, format, args);
  va_end(args);
  return out;
}

extern int vprintf(const char *format, va_list args) {
  return vfprintf(stdout, format, args);
}

extern int vsprintf(char *buffer, const char *format, va_list args) {
  FILE fraw;

  fraw.eof = 0;
  fraw.error = 0;
  fraw.ram_file = 1;
  fraw.file.buffer = buffer;
  return vfprintf(&fraw, format, args);
}

extern int vfprintf(FILE *f, const char *format, va_list args) {
  /* TODO: implement this function. */
  (void) args;

  fputs(format, f);
  return strlen(format);
}

static int vfscanf(FILE *, const char *, va_list);

extern int fscanf(FILE *f, const char *format, ...) {
  va_list args;
  int out;

  va_start(args, format);
  out = vfscanf(f, format, args);
  va_end(args);
  return out;
}

extern int scanf(const char *format, ...) {
  va_list args;
  int out;

  va_start(args, format);
  out = vfscanf(stdout, format, args);
  va_end(args);
  return out;
}

extern int sscanf(char *buffer, const char *format, ...) {
  FILE fraw;
  va_list args;
  int out;

  fraw.eof = 0;
  fraw.error = 0;
  fraw.ram_file = 1;
  fraw.file.buffer = buffer;
  va_start(args, format);
  out = vfscanf(&fraw, format, args);
  va_end(args);
  return out;
}

static int vfscanf(FILE *f, const char *format, va_list args) {
  /* TODO: implement this function. */
  (void) f;
  (void) format;
  (void) args;

  return 0;
}

extern char *fgets(char *str, int count, FILE *f) {
  int chars_read;

  if ((chars_read = fread(str, 1, count, f)) == 0)
    if (feof(f) || ferror(f))
      return (char *) 0;

  str[chars_read] = '\0';
  return str;
}

extern long int ftell(FILE *f) {
  if (f->ram_file)
    return (long int) -1;
  else
    return (long int) __lseek(f->file.unix_file.fd, 0, SEEK_CUR);
}

extern int fseek(FILE *f, long int offset, int origin) {
  if (f->ram_file) {
    if (origin == SEEK_CUR) {
      f->file.buffer += offset;
      return 0;
    } else {
      return EOF;
    }
  } else {
    (void) __lseek(f->file.unix_file.fd, offset, origin);
    return 0;
  }
}

extern int fgetpos(FILE *f, fpos_t *pos) {
  if ((*pos = ftell(f)) == -1)
    return EOF;
  else
    return 0;
}

extern int fsetpos(FILE *f, const fpos_t *pos) {
  if (fseek(f, *pos, SEEK_SET))
    return EOF;
  else
    return 0;
}

extern void rewind(FILE *f) {
  fseek(f, 0, SEEK_SET);
  clearerr(f);
}

extern void clearerr(FILE *f) {
  f->error = 0;
}

extern int ungetc(int ch, FILE *f) {
  if (ch == EOF || f->ram_file || f->file.unix_file.last_char != EOF) {
    return EOF;
  } else {
    f->file.unix_file.last_char = ch;
    return ch;
  }
}

extern char *gets(char *buffer) {
  int c;
  char *bufferp;

  bufferp = buffer;
  while ((c = getchar()) != EOF)
    *bufferp++ = (char) c;

  if (ferror(stdin))
    return (char *) 0;
  else
    return buffer;
}

extern FILE *freopen(const char *filename, const char *mode, FILE *f) {
  FILE *g;

  fclose(f);
  if (!filename)
    return (FILE *) 0;
  if (!(g = fopen(filename, mode)))
    return (FILE *) 0;
  memcpy(f, g, sizeof(*g));
  free(g);

  return f;
}
