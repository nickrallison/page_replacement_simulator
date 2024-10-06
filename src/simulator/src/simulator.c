#include "simulator.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "page.h"

// #### page QUEUE ####
// page_queue_t page_queue_new(int (*get_value)(const page_record_t *)) {
//     page_records_t page_records = page_records_new();
//     page_queue_t page_queue = {get_value, page_records};
//     return page_queue;
// }
//
// int page_record_exchange(page_record_t *a, page_record_t *b) {
//     page_record_t temp = *a;
//     *a = *b;
//     *b = temp;
//     return 0;
// }
//
// int page_queue_add(page_queue_t *page_queue, page_record_t page) {
//     page_records_append(&page_queue->page_queue, page);
//
//     int index = page_queue->page_queue.size - 1;
//     uint32_t parent_index = (index - 1) / 2;
//
//     int key = page_queue->get_value(&page);
//     // Min Int
//     int parent_key = -2147483648;
//     if (index > 0) {
//         parent_key = page_queue->get_value(&page_queue->page_queue.page_records[parent_index]);
//         while (index > 0 && key > parent_key) {
//             page_record_exchange(&page_queue->page_queue.page_records[index],
//                                 &page_queue->page_queue.page_records[parent_index]);
//             index = parent_index;
//             parent_index = (index - 1) / 2;
//             key = page_queue->get_value(&page_queue->page_queue.page_records[index]);
//             parent_key = -2147483648;
//             if (index > 0) {
//                 parent_key = page_queue->get_value(&page_queue->page_queue.page_records[parent_index]);
//             }
//         }
//     }
//     return 0;
// }
//
// int page_queue_max_heapify(page_queue_t *page_queue, uint32_t index) {
//     uint32_t left = 2 * index + 1;
//     uint32_t right = 2 * index + 2;
//     uint32_t largest = index;
//
//     if (left < page_queue->page_queue.size &&
//         page_queue->get_value(&page_queue->page_queue.page_records[left]) >
//         page_queue->get_value(&page_queue->page_queue.page_records[largest])) {
//         largest = left;
//     }
//
//     if (right < page_queue->page_queue.size &&
//         page_queue->get_value(&page_queue->page_queue.page_records[right]) >
//         page_queue->get_value(&page_queue->page_queue.page_records[largest])) {
//         largest = right;
//     }
//
//     if (largest != index) {
//         page_record_exchange(&page_queue->page_queue.page_records[index],
//                              &page_queue->page_queue.page_records[largest]);
//         page_queue_max_heapify(page_queue, largest);
//     }
//     return 0;
// }
// page_record_t page_queue_pop_top(page_queue_t *page_queue) {
//     page_record_t top = page_queue->page_queue.page_records[0];
//     page_queue->page_queue.page_records[0] = page_queue->page_queue.page_records[page_queue->page_queue.size - 1];
//     page_queue->page_queue.size--;
//     page_queue_max_heapify(page_queue, 0);
//     return top;
// }

// #### SIMULATOR ####
// typedef struct simulator_t {
//     int current_index;
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
// } simulator_t;
simulator_t simulator_new(page_records_t* page_records_in_order, uint8_t sim_type, uint32_t cache_capacity, uint32_t interrupt_interval) {
    int current_index = 0;
    uint32_t cache_size = 0;
    page_record_t *page_cache = malloc(cache_capacity * sizeof(page_record_t));
    for (int i = 0; i < cache_size; i++) {
        page_cache[i] = page_record_new(0, 0);
    }
    uint32_t page_faults = 0;
    uint32_t write_backs = 0;
    uint32_t clock_index = 0;
    uint32_t interrupt_counter = 0;
    simulator_t simulator = {current_index, page_records_in_order, cache_capacity, cache_size, page_cache, page_faults, write_backs, sim_type, clock_index, interrupt_interval, interrupt_counter};
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
            break;
        }
    }

    if (!found) {
        // if the cache is not found, add the page to the cache and increment page_faults
        simulator->page_faults++;
        if (simulator->cache_size < simulator->cache_capacity) {
            simulator->page_cache[simulator->cache_size] = page;
            simulator->cache_size++;
        } else {
            // if the cache is full, replace the page that will be used the farthest in the future
            uint32_t farthest_page_cache_index = 0;
            uint32_t farthest_page_index = 0;
            for (int i = 0; i < simulator->cache_size; i++) {
                for (uint32_t j = simulator->current_index; j < simulator->page_records_in_order->size; j++) {
                    // i -> cache index, j -> page_records index
                    if (simulator->page_cache[i].page_number == simulator->page_records_in_order->page_records[j].page_number) {
                        if (j >= farthest_page_index) {
                            farthest_page_cache_index = i;
                            farthest_page_index = j;
                        }
                        break;
                    }
                }
            }
            // when evicting a page, check if it is dirty
            if (simulator->page_cache[farthest_page_cache_index].dirty) {
                simulator->write_backs++;
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
            simulator->page_cache[i].last_access_time = simulator->current_index;
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

// Clock
// ####
// 1. Find out if the page is in the cache
// 2. If it is, do set the most_recent_access_time to the current time and set the second_chance bit to 1
// 3. If it is not, replace the page that has the second chance bit set to 0
void simulator_time_step_clk(simulator_t* simulator) {

    if (simulator->interrupt_interval > 0) {
        simulator->interrupt_counter++;
        if (simulator->interrupt_counter == simulator->interrupt_interval) {
            for (int i = 0; i < simulator->cache_size; i++) {
                simulator->page_cache[i].second_chance = 0;
            }
            simulator->interrupt_counter = 0;
        }
    }

    page_record_t page = simulator->page_records_in_order->page_records[simulator->current_index];
    uint32_t page_number = page.page_number;
    uint8_t found = 0;
    for (int i = 0; i < simulator->cache_size; i++) {
        if (simulator->page_cache[i].page_number == page_number) {
            found = 1;
            break;
        }
    }

    if (!found) {
        // if the cache is not found, add the page to the cache and increment page_faults
        simulator->page_faults++;
        if (simulator->cache_size < simulator->cache_capacity) {
            page.arrival_time = simulator->current_index;
            simulator->page_cache[simulator->cache_size] = page;
            simulator->page_cache[simulator->cache_size].second_chance = 1;
            simulator->cache_size++;
        } else {
            // if the cache is full, replace the page that has the second chance bit set to 0
            while (1) {
                if (simulator->page_cache[simulator->clock_index].second_chance) {
                    simulator->page_cache[simulator->clock_index].second_chance = 0;
                    simulator->clock_index = (simulator->clock_index + 1) % simulator->cache_size;
                } else {
                    break;
                }
            }
            // when evicting a page, check if it is dirty
            if (simulator->page_cache[simulator->clock_index].dirty) {
                simulator->write_backs++;
            }
            simulator->page_cache[simulator->clock_index] = page;
            simulator->page_cache[simulator->clock_index].second_chance = 1;
            simulator->clock_index = (simulator->clock_index + 1) % simulator->cache_size;
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
    // this while loop will cause problems if
    while (simulator->current_index < simulator->page_records_in_order->size) {
        simulator_time_step(simulator);
    }
    simulator_stats_t stats = {simulator->page_faults, simulator->write_backs};
    return stats;
}
