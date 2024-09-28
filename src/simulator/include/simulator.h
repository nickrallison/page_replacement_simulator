// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stddef.h>
#include <stdint.h>

#include "page.h"


// this is a heap struct to store the pages
typedef struct page_queue_t {
    int (*get_value)(const page_record_t *);
    page_records_t page_queue;
} page_queue_t;

page_queue_t page_queue_new(int (*get_value)(const page_record_t *));

int page_queue_add(page_queue_t *page_queue, page_record_t page);


/// @brief A struct to store the simulator
typedef struct simulator_t {
    int current_index;

    page_record_t current_page_option;
    uint8_t has_current_page;
    page_records_t page_records_in_order;

    uint32_t cache_size;
    page_queue_t cache;

    uint32_t page_faults;
    uint32_t write_backs;

    uint32_t jobs_remaining;

} simulator_t;

simulator_t simulator_new(page_records_t* page_records,
                          int (*get_value)(const page_record_t *), uint32_t cache_size);

page_records_t *simulator_run(simulator_t *simulator);


#endif // SIMULATOR_H
