
#ifndef REG_H
#define REG_H

#include <stddef.h>
#include <stdint.h>

typedef struct clock_register_t {
    uint32_t bit_count;
    uint8_t* bits;
} clock_register_t;

clock_register_t clock_register_new(uint32_t bit_count);
int clock_register_shift(clock_register_t *self);
int clock_register_set_front_one(clock_register_t *self);
int clock_register_reset(clock_register_t *self);
int clock_register_is_zero(clock_register_t *self);

#endif // REG_H