/* See LICENSE file for copyright and license details. */

/* include <stdint.h> */

typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef int64_t  S64;
typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef S8       B8;
typedef S16      B16;
typedef S32      B32;
typedef S64      B64;
typedef float    F32;
typedef double   F64;

#define internal static 
#define local_persist static
#define global_variable static 

#define Stmnt(S) do{ S }while(0)

#define AssertBreak() (*(int *)0 = 0)

#if ENABLE_ASSERT
# define Assert(c) Stmnt( if (!(c)){ AssertBreak(); })
#else
# define Assert(c)
#endif

#define ArrayCount(a) (sizeof(a)/sizeof(*(a)))

#define Min(a,b) (((a)<(b))?(a):(b))
#define Max(a,b) (((a)>(b))?(a):(b))
#define Clamp(a,x,b) (((x)<(a))?(a):\
                      ((b)<(x))?(b):(x))
