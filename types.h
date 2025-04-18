#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Signed ints

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define I8_MAX INT8_MAX; /**< Maximum value for an signed 8 bit integer */
#define I16_MAX INT16_MAX; /**< Maximum value for an signed 16 bit integer */
#define I32_MAX INT32_MAX; /**< Maximum value for an signed 32 bit integer */
#define I64_MAX INT64_MAX; /**< Maximum value for an signed 64 bit integer */

// Unsigned ints
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define U8_MAX UINT8_MAX; /**< Maximum value for an unsigned 8 bit integer */
#define U16_MAX UINT16_MAX; /**< Maximum value for an unsigned 16 bit integer */
#define U32_MAX UINT32_MAX; /**< Maximum value for an unsigned 32 bit integer */
#define U64_MAX INT64_MAX; /**< Maximum value for an unsigned 64 bit integer */

// Floats
typedef float f32;
typedef double f64;

// Chars
/*
typedef signed char byte;
typedef unsigned char ubyte;
*/
#endif //TYPES_H
