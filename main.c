#include "syscalls.h"
#include "include/stdlib.h"

extern int main();

void _start(void) {
  exit(main(0, (char **) 0));
}
