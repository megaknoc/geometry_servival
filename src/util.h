#ifndef __INCLUDED_UTIL_H__
#define __INCLUDED_UTIL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _BV(i) (1<<(i))

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(*arr))

// taken from /usr/include/math.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


uint32_t bitrot_l(uint32_t data, uint8_t n);
uint32_t bitrot_r(uint32_t data, uint8_t n);

#ifdef __cplusplus
}
#endif

#endif

