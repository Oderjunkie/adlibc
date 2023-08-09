.text
.global __syscall

# this routine converts user-level calling conventions into
# kernel-level calling conventions.
__syscall:
  mov %edi, %eax    # arg #1 (edi) -> syscall # (eax)
  mov %esi, %edi    # arg #2 (esi) -> arg #1 (edi)
  mov %edx, %esi    # arg #3 (edx) -> arg #2 (esi)
  mov %ecx, %edx    # arg #4 (ecx) -> arg #3 (edx)
  mov %r8d, %r10d   # arg #5 (r8d) -> arg #4 (r10d)
                    # if you've got a keen eye you'll have noticed
                    # that i said that ecx and r10d were both arg #4,
                    # that's because ecx is used for user-level functions
                    # and r10d is used for kernel-level functions (i.e. syscalls)
  mov %r9d, %r8d    # arg #6 (r9d) -> arg #5 (r8d)
  mov 8(%rsp), %r9d # arg #7 (stack) -> arg #6 (r9d)
  syscall           # return value is already in eax
  ret
