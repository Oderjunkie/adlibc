#include "syscalls.h"

extern ssize_t __read(int fd, void *buf, size_t len) {
  return (ssize_t) __syscall(_READ_SYSCALL_NUM, fd, buf, len);
}

extern ssize_t __write(int fd, const void *buf, size_t len) {
  return (ssize_t) __syscall(_WRITE_SYSCALL_NUM, fd, buf, len);
}

extern void *__mmap(void *addr, size_t len, int protections, int flags, int fd, off_t offset) {
  return (void *) __syscall(_MMAP_SYSCALL_NUM, addr, len, protections, flags, fd, offset);
}

extern void __munmap(void *addr, size_t len) {
  (void) __syscall(_MUNMAP_SYSCALL_NUM, addr, len);
}

extern void __exit(int code) {
  (void) __syscall(_EXIT_SYSCALL_NUM, code);
}
