#ifndef COMMON_H_
#define COMMON_H_
#include <stdint.h>
#include <assert.h>
#include "stm32h7xx.h"
/* atributes */

#define ASSERT(cond)            assert(cond)

#define ATTRIBUTE(...)        __attribute__ (( __VA_ARGS__ ))

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define ASM(instruction)    __asm volatile (instruction)

#define critical_start()    __disable_irq()
#define critical_end()      __enable_irq()

#endif /* COMMON_H_ */