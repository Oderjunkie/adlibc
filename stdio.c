#include "include/stdio.h"
#include "include/string.h"
#include "syscalls.h"

struct FILE {
  int fd;
  int last_char;
  unsigned int eof : 1;
  unsigned int error : 1;
};

static FILE _stdin = { 0, EOF, 0, 0 };
static FILE _stdout = { 1, EOF, 0, 0 };
static FILE _stderr = { 2, EOF, 0, 0 };
FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

extern int feof(FILE *f) { return f->eof; }
extern int ferror(FILE *f) { return f->error; }
static size_t frwerror(ssize_t read_bytes, size_t count, FILE *f) {
  if (read_bytes > 0) {
    f->eof = 0;
    f->error = 0;
    return read_bytes / count;
  } else if (read_bytes < 0) {
    f->eof = 0;
    f->error = 1;
    return 0;
  } else {
    f->eof = 1;
    f->error = 0;
    return 0;
  }
}

extern size_t fread(void *buffer, size_t size, size_t count, FILE *f) {
  return frwerror(__read(f->fd, buffer, size * count), count, f);
}

extern size_t fwrite(const void *buffer, size_t size, size_t count, FILE *f) {
  return frwerror(__write(f->fd, buffer, size * count), count, f);
}

extern int fgetc(FILE *f) {
  unsigned char out;
  if (fread(&out, 1, 1, f) < 1)
    return EOF;
  else
    return (int) out;
}

extern int fputc(int ch, FILE *f) {
  unsigned char ucch = (unsigned char) ch;
  if (fwrite(&ucch, 1, 1, f) == 1)
    return ch;
  else
    return EOF;
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
