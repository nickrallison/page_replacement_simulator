// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stddef.h>
#include <stdint.h>

#include "page.h"


typedef struct page_queue_t {
    // function pointer to compare two pages
    int (*compare)(const void *, const void *);

    // page_records struct to store the pages
    page_records_t page_queue;
} page_queue_t;

int page_queue_new(page_records_t page_records, int (*compare)(const void *, const void *));

int page_queue_add(page_queue_t *page_queue, page_record_t page);


/// @brief A struct to store the simulator
typedef struct simulator_t {
    uint32_t time_quantum;

    uint32_t current_time;
    int current_index;

    uint8_t has_current_page;
    page_record_t current_page_option;

    page_records_t page_records_in_order;

    page_queue_t page_queue;

    page_records_t page_completion_records;
    uint32_t jobs_remaining;
    uint32_t time_quantum_remaining;

} simulator_t;

simulator_t simulator_new(page_records_t* page_records,
                          int (*compare)(const void *, const void *));

page_records_t *simulator_run(simulator_t *simulator);


#endif // SIMULATOR_H
