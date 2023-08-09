#include <sys/types.h>

extern void *__syscall(int, ...);

enum {
  _READ_SYSCALL_NUM = 0,
  _WRITE_SYSCALL_NUM = 1,
  _OPEN_SYSCALL_NUM = 2,
  _CLOSE_SYSCALL_NUM = 3,
  _LSEEK_SYSCALL_NUM = 8,
  _MMAP_SYSCALL_NUM = 9,
  _MUNMAP_SYSCALL_NUM = 11,
  _EXIT_SYSCALL_NUM = 60
};

enum {
  PROT_READ = 0x01, PROT_WRITE = 0x02, PROT_EXEC = 0x04,
  MAP_PRIVATE = 0x02, MAP_ANONYMOUS = 0x20,
  O_RDONLY = 00, O_WRONLY = 01, O_RDWR = 02,
  O_APPEND = 02000, O_CREAT = 0100, O_TRUNC = 01000,
  S_IROTH = 0400,
  SEEK_SET = 0, SEEK_CUR = 1, SEEK_END = 2
};

extern ssize_t __read(int, void *, size_t);
extern ssize_t __write(int, const void *, size_t);
extern int __open(const char *, int, mode_t);
extern int __close(int);
extern off_t __lseek(int, off_t, int);
extern void *__mmap(void *, size_t, int, int, int, off_t);
extern void __munmap(void *, size_t);
extern void __exit(int);

