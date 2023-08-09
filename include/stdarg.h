typedef void *va_list;
#define va_start(list, arg) list = (void *) 0
#define va_arg(list, T) *(((T) *) list)
