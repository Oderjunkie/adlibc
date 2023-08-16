#define TWOS_COMPLEMENT
/* #define SIGN_MAGNITUDE */

#ifndef __GNUC__
#define __CHAR_BIT__   8
#define __SHRT_WIDTH__ 16
#define __INT_WIDTH__  32
#define __LONG_WIDTH__ 32
/* #define __CHAR_UNSIGNED__ */
#endif

#define CHAR_BIT  __CHAR_BIT__
#define UCHAR_MAX (1UL << CHAR_BIT           - 1UL)
#define USHRT_MAX (1UL << __SHRT_WIDTH__     - 1UL)
#define UINT_MAX  (1UL << __INT_WIDTH__      - 1UL)
#define ULONG_MAX (1UL << __LONG_WIDTH__     - 1UL)

#define SCHAR_MAX (1UL << (CHAR_BIT-1)       - 1UL)
#define SHRT_MAX  (1UL << (__SHRT_WIDTH__-1) - 1UL)
#define INT_MAX   (1UL << (__INT_WIDTH__-1) - 1UL)
#define LONG_MAX  (1UL << (__LONG_WIDTH__-1) - 1UL)

#if defined(TWOS_COMPLEMENT)
#define SCHAR_MIN -SCHAR_MAX-1
#define SHRT_MIN -SHRT_MAX-1
#define INT_MIN -INT_MAX-1
#define LONG_MIN -LONG_MAX-1
#elif defined(SIGN_MAGNITUDE)
#define SCHAR_MIN -SCHAR_MAX
#define SHRT_MIN -SHRT_MAX
#define INT_MIN -INT_MAX
#define LONG_MIN -LONG_MAX
#else
#error "<limits.h> requires either a TWOS COMPLEMENT system or a SIGN*MAGNITUDE system"
#endif

#ifdef __CHAR_UNSIGNED__
#define CHAR_MAX UCHAR_MAX
#define CHAR_MIN 0UL
#else
#define CHAR_MAX SCHAR_MAX
#define CHAR_MIN SCHAR_MIN
#endif

