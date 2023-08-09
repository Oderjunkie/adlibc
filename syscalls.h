#include <sys/types.h>

extern void *__syscall(int, ...);

enum {
  _READ_SYSCALL_NUM = 0,
  _WRITE_SYSCALL_NUM = 1,
  _MMAP_SYSCALL_NUM = 9,
  _MUNMAP_SYSCALL_NUM = 11,
  _EXIT_SYSCALL_NUM = 60
};

extern ssize_t __read(int, void *, size_t);
extern ssize_t __write(int, const void *, size_t);
extern void *__mmap(void *, size_t, int, int, int, off_t);
extern void __munmap(void *, size_t);
extern void __exit(int);
