#include "simulator.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "page.h"

// #### page QUEUE ####
int page_queue_new(page_records_t page_records,
                      int (*compare)(const void *, const void *)) {
    page_queue_t page_queue = {compare, page_records};
    return 0;
}

int page_queue_add(page_queue_t *page_queue, page_record_t page) {
    page_records_append(&page_queue->page_queue, page);
    page_records_sort_by(&page_queue->page_queue, page_queue->compare);

    return 0;
}

page_record_t page_queue_pop(page_queue_t *page_queue) {
    page_record_t page = page_queue->page_queue.page_records[0];
    for (int i = 0; i < page_queue->page_queue.size - 1; i++) {
        page_queue->page_queue.page_records[i] =
                page_queue->page_queue.page_records[i + 1];
    }
    page_queue->page_queue.size--;
    return page;
}

// #### SIMULATOR ####
simulator_t simulator_new(page_records_t* page_records,
                          int (*compare)(const void *, const void *)) {

}

// 1. if another page is ready to run, add it to the page queue
// 2. if using preemptive scheduling, check if the current page should be
// added back to the page queue
// 3. if no page is running, get the next page from the page queue
// 4. run the page for one time step
// 5. if the page is done, add it to the completion records
// 6. increment the current time

int simulator_time_step(simulator_t *simulator) {
    // 1. if another page is ready to run, add it to the page queue
    // using the current index to keep track of the next page to add
    while (simulator->current_index < simulator->page_records_in_order.size &&
           simulator->page_records_in_order.page_records[simulator->current_index]
           .arrival_time == simulator->current_time) {
        simulator->page_records_in_order
                          .page_records[simulator->current_index].added_to_queue = simulator->current_time;
        page_queue_add(&simulator->page_queue,
                          simulator->page_records_in_order
                          .page_records[simulator->current_index]);
        simulator->current_index++;
    }

    // 2. if using preemptive scheduling, check if the current page should be
    // added back to the page queue
    if (simulator->time_quantum && simulator->has_current_page &&
        simulator->current_page_option.running_cpu_burst > 0 && simulator->time_quantum_remaining == 0) {
        simulator->current_page_option.added_to_queue = simulator->current_time;
        simulator->current_page_option.last_preempted = simulator->current_time;
        page_queue_add(&simulator->page_queue,
                          simulator->current_page_option);

        simulator->has_current_page = 0;
        simulator->time_quantum_remaining = simulator->time_quantum;
    }

    // 3. if no page is running, get the next page from the page queue
    if (!simulator->has_current_page) {
        if (simulator->page_queue.page_queue.size > 0) {
            simulator->current_page_option =
                    page_queue_pop(&simulator->page_queue);
            simulator->has_current_page = 1;
            simulator->time_quantum_remaining = simulator->time_quantum;
        }
    }

    // 4. run the page for one time step
    simulator->current_page_option.running_cpu_burst--;
    simulator->time_quantum_remaining--;
    if (simulator->current_page_option.has_started == 0) {
        simulator->current_page_option.start_time = simulator->current_time;
        simulator->current_page_option.has_started = 1;
    }
    if (simulator->current_page_option.running_time_until_first_response > 0) {
        simulator->current_page_option.running_time_until_first_response--;
    }


    // 5. if the page responded, record the first response time
    if (simulator->has_current_page &&
        simulator->current_page_option.running_time_until_first_response == 0 && simulator->current_page_option.first_response_time == 0) {
        simulator->current_page_option.first_response_time = simulator->current_time + 1;
        // printf("Time: %d, Burst Left: %d, Response Time: %d\n",
        //        simulator->current_time + 1, simulator->current_page_option.running_cpu_burst, simulator->current_page_option.first_response_time);
    }

    // 6. if the page is done, add it to the completion records
    if (simulator->has_current_page &&
        simulator->current_page_option.running_cpu_burst == 0) {
        simulator->current_page_option.completion_time = simulator->current_time + 1;
        simulator->has_current_page = 0;
        simulator->jobs_remaining--;

        simulator->seq_pages[simulator->seq_page_index] = simulator->current_page_option.page;
        simulator->seq_page_index++;

        // tau_n+1 = alpha * t_n + (1 - alpha) * tau_n


        // tau_n = simulator->exp_time_remaining_estimate
        // t_n = simulator->current_page_option.actual_cpu_burst
        simulator->current_page_option.exp_time_remaining_chart[simulator->current_page_option.page] =
                simulator->alpha * (float) simulator->current_page_option.actual_cpu_burst +
                (1 - simulator->alpha) * simulator->current_page_option.exp_time_remaining_chart[simulator->current_page_option.page];

        page_records_append(&simulator->page_completion_records,
                           simulator->current_page_option);
    }

    // 7. increment the current time
    simulator->current_time++;
    return 0;
}

page_records_t *simulator_run(simulator_t *simulator) {
    // this while loop will cause problems if
    while (simulator->jobs_remaining > 0) {
        simulator_time_step(simulator);
    }
    simulator->page_completion_records.seq_pages = simulator->seq_pages;
    simulator->page_completion_records.seq_page_index = simulator->seq_page_index;
    return &simulator->page_completion_records;
}
