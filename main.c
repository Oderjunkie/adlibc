#include "syscalls.h"

extern int main();

void _start(void) {
  __exit(main(0, (char **) 0));
}
