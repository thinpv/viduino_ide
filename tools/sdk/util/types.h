#ifndef __ARM32_TYPES_H__
#define __ARM32_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef signed char				s8_t;
typedef unsigned char			u8_t;

typedef signed short			s16_t;
typedef unsigned short			u16_t;

typedef signed int				s32_t;
typedef unsigned int			u32_t;

typedef signed long long		s64_t;
typedef unsigned long long		u64_t;

typedef signed char				s8;
typedef unsigned char			u8;

typedef signed short			s16;
typedef unsigned short			u16;

typedef signed int				s32;
typedef unsigned int			u32;

typedef signed long long		s64;
typedef unsigned long long		u64;

typedef signed long long		intmax_t;
typedef unsigned long long		uintmax_t;

typedef signed int				ptrdiff_t;
typedef signed int				intptr_t;
typedef unsigned int 			uintptr_t;

typedef unsigned int			size_t;
typedef signed int				ssize_t;

typedef signed long long		loff_t;

typedef signed int				bool_t;

typedef signed int				register_t;
typedef unsigned int			irq_flags_t;

typedef unsigned int			virtual_addr_t;
typedef unsigned int			virtual_size_t;
typedef unsigned int			physical_addr_t;
typedef unsigned int			physical_size_t;

typedef volatile unsigned char vuint8_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned int vuint32_t;
typedef volatile unsigned long long vuint64_t;

typedef void *            PVOID;    ///< Define void pointer data type
typedef void              VOID;     ///< Define void data type
typedef char              BOOL;     ///< Define bool data type
typedef char *            PBOOL;    ///< Define bool pointer data type

typedef char              INT8;     ///< Define 8-bit singed data type
typedef char              CHAR;     ///< Define char data type
typedef char *            PINT8;    ///< Define 8-bit singed pointer data type
typedef char *            PCHAR;    ///< Define char pointer data type
typedef unsigned char     UINT8;    ///< Define 8-bit unsigned data type
typedef unsigned char     UCHAR;    ///< Define char unsigned data type
typedef unsigned char *   PUINT8;   ///< Define 8-bit unsigned pointer data type
typedef unsigned char *   PUCHAR;   ///< Define char unsigned pointer data type
typedef char *            PSTR;     ///< Define string pointer data type
typedef const char *      PCSTR;    ///< Define constant string pointer data type

typedef short             SHORT;    ///< Define short signed data type
typedef short *           PSHORT;   ///< Define short signed pointer data type
typedef unsigned short    USHORT;   ///< Define short unsigned data type
typedef unsigned short *  PUSHORT;  ///< Define short unsigned pointer data type

typedef short             INT16;    ///< Define 16-bit signed data type
typedef short *           PINT16;   ///< Define 16-bit signed pointer data type
typedef unsigned short    UINT16;   ///< Define 16-bit unsigned data type
typedef unsigned short *  PUINT16;  ///< Define 16-bit unsigned pointer data type

typedef int               INT;      ///< Define integer signed data type
typedef int *             PINT;     ///< Define integer signed pointer data type
typedef unsigned int      UINT;     ///< Define integer unsigned data type
typedef unsigned int *    PUINT;    ///< Define integer unsigned pointer data type

typedef int               INT32;    ///< Define 32-bit signed data type
typedef int *             PINT32;   ///< Define 32-bit signed pointer data type
typedef unsigned int      UINT32;   ///< Define 32-bit unsigned data type
typedef unsigned int *    PUINT32;  ///< Define 32-bit unsigned pointer data type

typedef __int64           INT64;    ///< Define 64-bit signed data type
typedef unsigned __int64  UINT64;   ///< Define 64-bit unsigned data type

typedef float             FLOAT;    ///< Define float data type
typedef float *           PFLOAT;   ///< Define float pointer data type

typedef double            DOUBLE;   ///< Define double data type
typedef double *          PDOUBLE;  ///< Define double pointer data type

typedef int               SIZE_T;   ///< Define size of data type

typedef unsigned char     REG8;     ///< Define 8-bit register data type
typedef unsigned short    REG16;    ///< Define 16-bit register data type
typedef unsigned int      REG32;    ///< Define 32-bit register data type

typedef struct {
	volatile long counter;
} atomic_t;

typedef struct {
	volatile long lock;
} spinlock_t;

#define   Successful  0         ///< Function return value success
#define   Fail        1         ///< Function return value failed

#ifndef FALSE
#      define  FALSE   (0)
#endif

#ifndef TRUE
#      define  TRUE    (1)
#endif

#ifndef NULL
#      define  NULL 0
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ARM32_TYPES_H__ */
