//
// Created by Nick Allison on 2024-10-07.
//

#include "reg.h"

#include <stdlib.h>

// typedef struct clock_register_t {
//     uint32_t bit_count;
//     uint8_t* bits;
// } clock_register_t;
//
// clock_register_t clock_register_new(uint32_t clock_period, uint32_t bit_count);
// int clock_register_shift(clock_register_t *self);
// int clock_register_set_front_one(clock_register_t *self);
// int clock_register_reset(clock_register_t *self);
// int clock_register_is_zero(clock_register_t *self);

clock_register_t clock_register_new(uint32_t bit_count) {
    clock_register_t clock_register;
    clock_register.bit_count = bit_count;
    clock_register.bits = (uint8_t*)malloc(bit_count * sizeof(uint8_t));
    for (int i = 0; i < bit_count; i++) {
        clock_register.bits[i] = 0;
    }
    return clock_register;
}

int clock_register_shift(clock_register_t *self) {
    for (int i = 0; i < self->bit_count - 1; i++) {
        self->bits[i] = self->bits[i + 1];
    }
    self->bits[self->bit_count - 1] = 0;
    return 0;
}

int clock_register_set_front_one(clock_register_t *self) {
    self->bits[0] = 1;
    return 0;
}

int clock_register_reset(clock_register_t *self) {
    for (int i = 0; i < self->bit_count; i++) {
        self->bits[i] = 0;
    }
    return 0;
}

int clock_register_is_zero(clock_register_t *self) {
    for (int i = 0; i < self->bit_count; i++) {
        if (self->bits[i] != 0) {
            return 0;
        }
    }
    return 1;
}