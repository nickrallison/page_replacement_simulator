// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stddef.h>
#include <stdint.h>

#include "page.h"

// register for clock algorithm
typedef struct clock_register_t {
    uint8_t* bits;
    uint32_t clock_period;
    uint32_t clock_count;
} clock_register_t;

clock_register_t clock_register_new(uint32_t clock_period, uint32_t bit_count);
int clock_register_inc(clock_register_t *self);


typedef struct simulator_stats_t {
    uint32_t page_faults;
    uint32_t write_backs;
} simulator_stats_t;

typedef struct simulator_t {
    int current_index;
    page_records_t *page_records_in_order;

    uint32_t cache_capacity;
    uint32_t cache_size;
    page_record_t *page_cache;

    uint32_t page_faults;
    uint32_t write_backs;

    uint8_t sim_type;

    // ####
    // Clock
    // ####
    uint32_t clock_index;

    uint32_t interrupt_interval;
    uint32_t interrupt_counter;
} simulator_t;

simulator_t simulator_new(page_records_t* page_records_in_order, uint8_t sim_type, uint32_t cache_capacity, uint32_t interrupt_interval);

simulator_stats_t simulator_run(simulator_t *simulator);


#endif // SIMULATOR_H
