#include "util.h"

// Note:
// bit rotatet functions could be inlined or replaced by equivalent assembler instructions.

/**
 * @brief Rotate bits left by n.
 */
uint32_t bitrot_l(uint32_t data, uint8_t n)
{
    if (n > 32) {
        n /= 32;
    }
    uint32_t tmp;
    tmp = data << n;
    tmp |= (data >> (32-n));
    return tmp;
}

/**
 * @brief Rotate bits right by n.
 */
uint32_t bitrot_r(uint32_t data, uint8_t n)
{
    if (n > 32) {
        n /= 32;
    }
    uint32_t tmp;
    tmp = data >> n;
    tmp |= (data << (32-n));
    return tmp;
}
