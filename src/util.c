#include "util.h"

sfmt_t randomGenerator;

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

void initRandomGenerator()
{
    time_t t;
    time(&t);
    sfmt_init_gen_rand(&randomGenerator, (uint32_t)t);
}

uint32_t generateRandomUInt32()
{
    return sfmt_genrand_uint32(&randomGenerator);
}
