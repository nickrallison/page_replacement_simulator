#include "simulator.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "page.h"

// #### page QUEUE ####
page_queue_t page_queue_new(int (*get_value)(const page_record_t *)) {
    page_records_t page_records = page_records_new();
    page_queue_t page_queue = {get_value, page_records};
    return page_queue;
}

int page_record_exchange(page_record_t *a, page_record_t *b) {
    page_record_t temp = *a;
    *a = *b;
    *b = temp;
    return 0;
}

int page_queue_add(page_queue_t *page_queue, page_record_t page) {
    page_records_append(&page_queue->page_queue, page);

    int index = page_queue->page_queue.size - 1;
    uint32_t parent_index = (index - 1) / 2;

    int key = page_queue->get_value(&page);
    // Min Int
    int parent_key = -2147483648;
    if (index > 0) {
        parent_key = page_queue->get_value(&page_queue->page_queue.page_records[parent_index]);
        while (index > 0 && key > parent_key) {
            page_record_exchange(&page_queue->page_queue.page_records[index],
                                &page_queue->page_queue.page_records[parent_index]);
            index = parent_index;
            parent_index = (index - 1) / 2;
            key = page_queue->get_value(&page_queue->page_queue.page_records[index]);
            parent_key = -2147483648;
            if (index > 0) {
                parent_key = page_queue->get_value(&page_queue->page_queue.page_records[parent_index]);
            }
        }
    }
    return 0;
}

int page_queue_max_heapify(page_queue_t *page_queue, uint32_t index) {
    uint32_t left = 2 * index + 1;
    uint32_t right = 2 * index + 2;
    uint32_t largest = index;

    if (left < page_queue->page_queue.size &&
        page_queue->get_value(&page_queue->page_queue.page_records[left]) >
        page_queue->get_value(&page_queue->page_queue.page_records[largest])) {
        largest = left;
    }

    if (right < page_queue->page_queue.size &&
        page_queue->get_value(&page_queue->page_queue.page_records[right]) >
        page_queue->get_value(&page_queue->page_queue.page_records[largest])) {
        largest = right;
    }

    if (largest != index) {
        page_record_exchange(&page_queue->page_queue.page_records[index],
                             &page_queue->page_queue.page_records[largest]);
        page_queue_max_heapify(page_queue, largest);
    }
    return 0;
}
page_record_t page_queue_pop_top(page_queue_t *page_queue) {
    page_record_t top = page_queue->page_queue.page_records[0];
    page_queue->page_queue.page_records[0] = page_queue->page_queue.page_records[page_queue->page_queue.size - 1];
    page_queue->page_queue.size--;
    page_queue_max_heapify(page_queue, 0);
    return top;
}

// #### SIMULATOR ####
simulator_t simulator_new(page_records_t* page_records,
                          int (*get_value)(const page_record_t *), uint32_t cache_size) {
    int current_index = 0;

    page_record_t current_page_option = {0};
    uint8_t has_current_page = 0;
    page_records_t page_records_in_order = *page_records;
    page_queue_t cache = page_queue_new(get_value);

    uint32_t page_faults = 0;
    uint32_t write_backs = 0;

    uint32_t jobs_remaining = page_records_in_order.size;

    simulator_t simulator = {current_index, current_page_option,
                             has_current_page, page_records_in_order, cache_size, cache,
                             page_faults, write_backs, jobs_remaining};
    return simulator;

}

// 1. Grab the next page from the page_records_in_order (don't have to pop it off, just copy it from it s index)
// 2. If the cache is full, pop the top page off the cache and record that it needed a write back if its dirty
// 3. Insert the new page into the cache, on entrance to the queue record whether the page was a page fault

int simulator_time_step(simulator_t *simulator) {
    // 1. Grab the next page from the page_records_in_order (don't have to pop it off, just copy it from it s index)
    simulator->current_page_option = simulator->page_records_in_order.page_records[simulator->current_index];
    simulator->has_current_page = 1;
    simulator->current_page_option.arrival_time = simulator->current_index;
    // simulator->current_page_option.arrival_time = simulator->current_index;
    // page_queue_add(&simulator->cache, simulator->current_page_option);

    // 2. If the cache is full, pop the top page off the cache and record that it needed a write back if its dirty
    // if (simulator->cache.page_queue.size == simulator->cache_size) {
    //     page_record_t top = page_queue_pop_top(&simulator->cache);
    //     if (top.dirty) {
    //         simulator->write_backs++;
    //     }
    // }

    // 3. Insert the new page into the cache, on entrance to the queue record whether the page was a page fault
    // page_queue_add(&simulator->cache, simulator->current_page_option);
    // if not in the cache, it is a page fault
    uint32_t page_number = simulator->current_page_option.page_number;
    uint8_t has_page = 0;
    uint32_t cache_size = simulator->cache.page_queue.size;
    for (int i = 0; i < cache_size; i++) {
        uint32_t cache_page_number = simulator->cache.page_queue.page_records[i].page_number;
        if (cache_page_number == page_number) {
            // if the page is in the cache, it is not a page fault
            has_page = 1;
            break;
        }
    }
    if (!has_page) {
        simulator->page_faults++;
        if (simulator->cache.page_queue.size == simulator->cache_size) {
            page_record_t top = page_queue_pop_top(&simulator->cache);
            if (top.dirty) {
                simulator->write_backs++;
            }
        }
        page_queue_add(&simulator->cache, simulator->current_page_option);

    } else {
        // if the page is in the cache, it is not a page fault
    }




    // End of function
    simulator->current_index++;
    return 0;
}

page_records_t simulator_run(simulator_t *simulator) {
    // this while loop will cause problems if
    while (simulator->current_index < simulator->page_records_in_order.size) {
        simulator_time_step(simulator);
    }
    page_records_t page_records = simulator->page_records_in_order;
    page_records.page_faults = simulator->page_faults;
    page_records.write_backs = simulator->write_backs;
    return page_records;
}
