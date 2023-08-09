.text
.global __syscall

# this routine converts user-level calling conventions into
# kernel-level calling conventions.
__syscall:
  mov %rdi, %rax    # arg #1 (rdi) -> syscall # (rax)
  mov %rsi, %rdi    # arg #2 (rsi) -> arg #1 (rdi)
  mov %rdx, %rsi    # arg #3 (rdx) -> arg #2 (rsi)
  mov %rcx, %rdx    # arg #4 (rcx) -> arg #3 (rdx)
  mov %r8, %r10     # arg #5 (r8) -> arg #4 (r10)
                    # if you've got a keen eye you'll have noticed
                    # that i said that rcx and r10 were both arg #4,
                    # that's because rcx is used for user-level functions
                    # and r10 is used for kernel-level functions (i.e. syscalls)
  mov %r9, %r8      # arg #6 (r9) -> arg #5 (r8)
  mov 8(%rsp), %r9  # arg #7 (stack) -> arg #6 (r9)
  syscall           # return value is already in eax
  ret
