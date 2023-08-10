#ifdef __GNUC__
  typedef __builtin_va_list va_list;
  #define va_start(list, arg) __builtin_va_start(list, arg)
  #define va_arg(list, T) __builtin_va_arg(list, T)
  #define va_end(list) __builtin_va_end(list)
#else
  /* pray that the argument passed to `va_arg` happens */
  /* to be on the stack anyway according to the ABI    */
  typedef void *va_list;
  #define va_start(list, arg) ((list) = ((char *) &(arg) + sizeof(arg)))
  #define va_arg(list, T) ((void) ((list) = ((char *) (list) + sizeof(T))), ((T *) (list))[-1])
  #define va_end(list) ((list) = (void *) 0)
#endif
