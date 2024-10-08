#include "simulator.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "page.h"
#include "reg.h"

// #### SIMULATOR ####
simulator_t simulator_new(page_records_t* page_records_in_order, uint8_t sim_type, uint32_t cache_capacity, uint32_t interrupt_interval, uint32_t clock_reg_size) {
    int current_index = 0;
    uint32_t cache_size = 0;
    page_record_t *page_cache = malloc(cache_capacity * sizeof(page_record_t));
    for (int i = 0; i < cache_size; i++) {
        page_cache[i] = page_record_new(0, 0);
    }
    clock_register_t* clock_registers = malloc(cache_capacity * sizeof(clock_register_t));
    uint32_t page_faults = 0;
    uint32_t write_backs = 0;
    uint32_t clock_index = 0;
    uint32_t interrupt_counter = 0;
    simulator_t simulator = {current_index, clock_registers, page_records_in_order, cache_capacity, cache_size, page_cache, page_faults, write_backs, sim_type, clock_index, interrupt_interval, interrupt_counter, clock_reg_size};
    return simulator;
}


// Optimal
// ####
// 1. Find out if the page is in the cache
// 2. If it is, do set the most_recent_access_time to the current time and set the second_chance bit to 1
// 3. If it is not, find the page that will be used the farthest in the future
// 4. If the cache is full, replace the page that will be used the farthest in the future
void simulator_time_step_opt(simulator_t* simulator) {
    page_record_t page = simulator->page_records_in_order->page_records[simulator->current_index];
    uint32_t page_number = page.page_number;
    uint8_t found = 0;
    for (int i = 0; i < simulator->cache_size; i++) {
        if (simulator->page_cache[i].page_number == page_number) {
            found = 1;
            simulator->page_cache[i].dirty = page.dirty | simulator->page_cache[i].dirty;
            break;
        }
    }

    if (!found) {
        // printf("Miss! Page %d not found in cache\n", page_number);
        // if the cache is not found, add the page to the cache and increment page_faults
        simulator->page_faults++;
        if (simulator->cache_size < simulator->cache_capacity) {
            simulator->page_cache[simulator->cache_size] = page;
            simulator->cache_size++;
        } else {
            // if the cache is full, replace the page that will be used the farthest in the future
            uint32_t* farthest_page_cache_indices = (uint32_t*) malloc(simulator->cache_size * sizeof(uint32_t));
            uint32_t* farthest_page_queue_indices = (uint32_t*) malloc(simulator->cache_size * sizeof(uint32_t));
            uint32_t* farthest_page_numbers = (uint32_t*) malloc(simulator->cache_size * sizeof(uint32_t));
            for (int i = 0; i < simulator->cache_size; i++) {
                farthest_page_cache_indices[i] = i;
                farthest_page_queue_indices[i] = UINT32_MAX;
                farthest_page_numbers[i] = simulator->page_cache[i].page_number;
            }
            // uint32_t farthest_page_index
            for (int i = 0; i < simulator->cache_size; i++) {
                for (uint32_t j = simulator->current_index; j < simulator->page_records_in_order->size; j++) {
                    uint32_t page_number_watch = simulator->page_cache[i].page_number;
                    // uint32_t cache_index = i;
                    // printf("page_number_watch: %d\n", page_number_watch);
                    // printf("page: %d\n", simulator->page_records_in_order->page_records[j].page_number);
                    if (simulator->page_records_in_order->page_records[j].page_number == page_number_watch) {
                        // farthest_page_cache_indices[i] = cache_index;
                        farthest_page_queue_indices[i] = j;
                        farthest_page_numbers[i] = page_number_watch;
                        break;
                    }
                }
            }
            uint32_t farthest_page_queue_index = 0;
            uint32_t farthest_page_number_watch = 0;
            uint32_t farthest_page_cache_index = 0;
            for (int i = 0; i < simulator->cache_size; i++) {
                // if (farthest_page_cache_indices[i] > farthest_page_cache_index && farthest_page_cache_indices[i] != UINT32_MAX) {
                if (farthest_page_queue_indices[i] > farthest_page_queue_index) {
                    farthest_page_queue_index = farthest_page_queue_indices[i];
                    farthest_page_cache_index = i;
                    farthest_page_number_watch = farthest_page_numbers[i];
                    // printf("farthest_page_cache_index: %d\n", farthest_page_queue_index);
                    // printf("farthest_page_number_watch: %d\n", farthest_page_number_watch);
                }
            }

            // when evicting a page, check if it is dirty
            // printf("Eviction! %d replaced with %d ", simulator->page_cache[farthest_page_cache_index].page_number, page.page_number);
            if (simulator->page_cache[farthest_page_cache_index].dirty) {
                // printf("write back!\n");
                simulator->write_backs++;
            }
            else
            {
                printf("\n");
            }

            simulator->page_cache[farthest_page_cache_index] = page;
        }
    }
    simulator->current_index++;
}


// FIFO
// ####
// 1. Find out if the page is in the cache
// 2. If it is, do set the most_recent_access_time to the current time and set the second_chance bit to 1
// 3. If it is not, replace the page that has been in the cache the longest
// 4. Add the current time to the newest page
void simulator_time_step_fifo(simulator_t* simulator) {
    page_record_t page = simulator->page_records_in_order->page_records[simulator->current_index];
    uint32_t page_number = page.page_number;
    uint8_t found = 0;
    for (int i = 0; i < simulator->cache_size; i++) {
        if (simulator->page_cache[i].page_number == page_number) {
            found = 1;
            simulator->page_cache[i].dirty = page.dirty | simulator->page_cache[i].dirty;
            break;
        }
    }

    if (!found) {
        // if the cache is not found, add the page to the cache and increment page_faults
        simulator->page_faults++;
        if (simulator->cache_size < simulator->cache_capacity) {
            page.arrival_time = simulator->current_index;
            simulator->page_cache[simulator->cache_size] = page;
            simulator->cache_size++;
        } else {
            // if the cache is full, replace the oldest page
            uint32_t oldest_page_cache_index = 0;
            uint32_t oldest_page_arrival_time = UINT32_MAX;
            for (int i = 0; i < simulator->cache_size; i++) {
                if (simulator->page_cache[i].arrival_time < oldest_page_arrival_time) {
                    oldest_page_cache_index = i;
                    oldest_page_arrival_time = simulator->page_cache[i].arrival_time;
                }
            }
            // when evicting a page, check if it is dirty
            if (simulator->page_cache[oldest_page_cache_index].dirty) {
                simulator->write_backs++;
            }
            simulator->page_cache[oldest_page_cache_index] = page;
            simulator->page_cache[oldest_page_cache_index].arrival_time = simulator->current_index;
        }
    }
    simulator->current_index++;
}

// LRU
// ####
// 1. Find out if the page is in the cache
// 2. If it is, do set the most_recent_access_time to the current time and set the second_chance bit to 1
// 3. If it is not, replace the page that has been accessed the longest time ago
// 4. Add the current time to the newest page
void simulator_time_step_lru(simulator_t* simulator) {
    page_record_t page = simulator->page_records_in_order->page_records[simulator->current_index];
    uint32_t page_number = page.page_number;
    uint8_t found = 0;
    for (int i = 0; i < simulator->cache_size; i++) {
        if (simulator->page_cache[i].page_number == page_number) {
            found = 1;
            simulator->page_cache[i].last_access_time = simulator->current_index;
            simulator->page_cache[i].dirty = page.dirty | simulator->page_cache[i].dirty;
            break;
        }
    }

    if (!found) {
        // if the cache is not found, add the page to the cache and increment page_faults
        simulator->page_faults++;
        if (simulator->cache_size < simulator->cache_capacity) {
            page.arrival_time = simulator->current_index;
            page.last_access_time = simulator->current_index;
            simulator->page_cache[simulator->cache_size] = page;
            simulator->cache_size++;
        } else {
            // if the cache is full, replace the page that has been accessed the longest time ago
            uint32_t oldest_page_cache_index = 0;
            uint32_t oldest_page_arrival_time = UINT32_MAX;
            for (int i = 0; i < simulator->cache_size; i++) {
                if (simulator->page_cache[i].last_access_time < oldest_page_arrival_time) {
                    oldest_page_cache_index = i;
                    oldest_page_arrival_time = simulator->page_cache[i].last_access_time;
                }
            }
            // when evicting a page, check if it is dirty
            if (simulator->page_cache[oldest_page_cache_index].dirty) {
                simulator->write_backs++;
            }
            simulator->page_cache[oldest_page_cache_index] = page;
            simulator->page_cache[oldest_page_cache_index].last_access_time = simulator->current_index;
        }
    }
    simulator->current_index++;
}

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

// typedef struct simulator_t {
//     int current_index;
//     clock_register_t* clock_registers;
//     page_records_t *page_records_in_order;
//
//     uint32_t cache_capacity;
//     uint32_t cache_size;
//     page_record_t *page_cache;
//
//     uint32_t page_faults;
//     uint32_t write_backs;
//
//     uint8_t sim_type;
//
//     // ####
//     // Clock
//     // ####
//     uint32_t clock_index;
//
//     uint32_t interrupt_interval;
//     uint32_t interrupt_counter;
//     uint32_t clock_reg_size;
// } simulator_t;

// Clock
// ####
// 1. Find out if the page is in the cache
// 2. If it is, do set the most_recent_access_time to the current time and set a one in the clock register for that page
// 3. If it is not, step though the clocks, decrementing each bit until a zero is found
void simulator_time_step_clk(simulator_t* simulator) {
    page_record_t page = simulator->page_records_in_order->page_records[simulator->current_index];
    uint32_t page_number = page.page_number;
    uint8_t found = 0;


    // clock interrupt to reset the clock registers
    simulator->interrupt_counter++;
    if (simulator->interrupt_counter == simulator->interrupt_interval) {
        for (int i = 0; i < simulator->cache_size; i++) {
            clock_register_shift(&simulator->clock_registers[i]);
        }
        simulator->interrupt_counter = 0;
    }

    for (int i = 0; i < simulator->cache_size; i++) {
        if (simulator->page_cache[i].page_number == page_number) {
            simulator->page_cache[i].last_access_time = simulator->current_index;
            simulator->page_cache[i].dirty = page.dirty | simulator->page_cache[i].dirty;
            clock_register_set_front_one(&simulator->clock_registers[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        // if the cache is not found, add the page to the cache and increment page_faults
        simulator->page_faults++;
        if (simulator->cache_size < simulator->cache_capacity) {
            page.arrival_time = simulator->current_index;
            page.last_access_time = simulator->current_index;
            simulator->page_cache[simulator->cache_size] = page;
            simulator->clock_registers[simulator->cache_size] = clock_register_new(simulator->clock_reg_size);
            simulator->cache_size++;
        } else {
            // if the cache is full, replace the page that has a zero in the clock register
            uint32_t zero_page_cache_index = 0;
            for (int i = 0; i < simulator->cache_size; i++) {
                while (1) {
                    if (clock_register_is_zero(&simulator->clock_registers[simulator->clock_index])) {
                        zero_page_cache_index = simulator->clock_index;
                        break;
                    }
                    clock_register_shift(&simulator->clock_registers[simulator->clock_index]);
                    simulator->clock_index = (simulator->clock_index + 1) % simulator->cache_size;
                }
            }
            // when evicting a page, check if it is dirty
            if (simulator->page_cache[zero_page_cache_index].dirty) {
                simulator->write_backs++;
            }
            simulator->page_cache[zero_page_cache_index] = page;
            simulator->clock_registers[zero_page_cache_index] = clock_register_new(simulator->clock_reg_size);
            clock_register_set_front_one(&simulator->clock_registers[zero_page_cache_index]);
        }
    }
    simulator->current_index++;

}

void simulator_time_step(simulator_t* simulator) {
    switch (simulator->sim_type) {
        case 0:
            simulator_time_step_opt(simulator);
            break;
        case 1:
            simulator_time_step_fifo(simulator);
            break;
        case 2:
            simulator_time_step_lru(simulator);
            break;
        case 3:
            simulator_time_step_clk(simulator);
            break;
        default:
            fprintf(stderr, "Invalid sim_type: %d\n", simulator->sim_type);
            exit(1);
    }
}

simulator_stats_t simulator_run(simulator_t *simulator) {
    while (simulator->current_index < simulator->page_records_in_order->size) {
        simulator_time_step(simulator);
    }
    uint32_t dirty_in_cache = 0;
    for (int i = 0; i < simulator->cache_size; i++) {
        if (simulator->page_cache[i].dirty) {
            dirty_in_cache++;
        }
    }
    simulator_stats_t stats = {simulator->page_faults, simulator->write_backs + dirty_in_cache};
    return stats;
}
