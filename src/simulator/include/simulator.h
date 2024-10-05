// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stddef.h>
#include <stdint.h>

#include "page.h"


// this is a heap struct to store the pages
// typedef struct page_queue_t {
//     int (*get_value)(const page_record_t *);
//     page_records_t page_queue;
// } page_queue_t;
//
// page_queue_t page_queue_new(int (*get_value)(const page_record_t *));
//
// int page_queue_add(page_queue_t *page_queue, page_record_t page);

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
