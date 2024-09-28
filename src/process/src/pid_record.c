#include "pid_record.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ##### PID RECORD #####
// typedef struct pid_record_t {
//     uint32_t pid;
//     uint32_t arrival_time;
//     uint32_t time_until_first_response;
//     uint32_t actual_cpu_burst;
//     // ######
//     uint32_t start_time;
//     uint32_t running_cpu_burst;
//     uint32_t running_time_until_first_response;
//     uint32_t first_response_time;
//     uint32_t added_to_queue;
//     uint32_t* exp_time_remaining_chart;
//
//     // ###################
//     uint32_t completion_time;
//
//
// } pid_record_t;
pid_record_t pid_record_new(uint32_t pid, uint32_t arrival_time,
                            uint32_t time_until_first_response,
                            uint32_t actual_cpu_burst) {
    uint32_t start_time = 0;
    uint8_t has_started = 0;
    uint32_t running_cpu_burst = actual_cpu_burst;
    uint32_t running_time_until_first_response = time_until_first_response;
    uint32_t first_response_time = 0;
    uint32_t added_to_queue = 0;
    uint32_t exp_time_remaining = 10;
    uint32_t completion_time = 0;
    uint32_t last_preempted = 0;
    uint32_t* current_time = NULL;
    pid_record_t pid_record = {
        pid, arrival_time, time_until_first_response, actual_cpu_burst,
        start_time, has_started, running_cpu_burst, running_time_until_first_response, first_response_time,
        added_to_queue, NULL, last_preempted, current_time, completion_time};
    return pid_record;
}

int pid_record_print(pid_record_t *self) {
    printf("PID: %hd, Arrival: %hd, Time Until First Response: %hd, Actual CPU "
           "Burst: %hd\n",
           self->pid, self->arrival_time, self->time_until_first_response,
           self->actual_cpu_burst);
    return 0;
}

int pid_record_compare_arrival_time(const void *a, const void *b) {
    pid_record_t *pid_record_a = (pid_record_t *) a;
    pid_record_t *pid_record_b = (pid_record_t *) b;
    return pid_record_a->arrival_time - pid_record_b->arrival_time;
}

// ##### PID RECORDS CONTAINER #####
pid_records_t pid_records_new() {
    uint32_t size = 0;
    uint32_t capacity = 32;
    pid_record_t *records_array = malloc(capacity * sizeof(pid_record_t));
    pid_records_t pid_records = {size, capacity, records_array};
    return pid_records;
}

int pid_records_append(pid_records_t *self, const pid_record_t pid_record) {
    if (self->size == self->capacity - 1) {
        // malloc an array with double the space
        self->capacity *= 2;
        pid_record_t *new_buffer = malloc(self->capacity * sizeof(pid_record_t));

        // copy the elements
        memcpy(new_buffer, self->pid_records, self->size * sizeof(pid_record_t));

        // free the old array
        free(self->pid_records);

        // copy the new buffer to self->pid_records
        self->pid_records = new_buffer;
    }

    self->pid_records[self->size] = pid_record;
    self->size++;
    return 0;
}

int pid_records_print(const pid_records_t *self) {
    for (int i = 0; i < self->size; i++) {
        printf("PID: %d, Arrival: %d, Time Until First Response: %d, Actual CPU "
               "Burst: %d\n",
               self->pid_records[i].pid, self->pid_records[i].arrival_time,
               self->pid_records[i].time_until_first_response,
               self->pid_records[i].actual_cpu_burst);
    }
    return 0;
}

pid_records_t create_pid_records() {
    uint32_t pid;
    uint32_t arrival_time;
    uint32_t time_until_first_response;
    uint32_t actual_cpu_burst;

    pid_records_t pid_records = pid_records_new();

    // get rid of the first line
    char *first_line_char = malloc(sizeof(char));
    *first_line_char = '\0';
    while (*first_line_char != '\n') {
        *first_line_char = getchar();
    }

    // Reads stdin line by line and creates a pid_record_t for each line
    while (scanf("%d,%d,%d,%d\n", &pid, &arrival_time,
                 &time_until_first_response, &actual_cpu_burst) != EOF) {
        pid_record_t pid_record = pid_record_new(
            pid, arrival_time, time_until_first_response, actual_cpu_burst);
        pid_records_append(&pid_records, pid_record);
    }
    return pid_records;
}

pid_records_t create_pid_records_no_stdin(const char* input) {
    uint32_t pid;
    uint32_t arrival_time;
    uint32_t time_until_first_response;
    uint32_t actual_cpu_burst;

    pid_records_t pid_records = pid_records_new();

    // get rid of the first line
    const char *current = input;
    while (*current != '\n' && *current != '\0') {
        current++;
    }
    if (*current == '\n') {
        current++;
    }


    // Reads stdin line by line and creates a pid_record_t for each line
    while (sscanf(current, "%d,%d,%d,%d\n", &pid, &arrival_time,
                  &time_until_first_response, &actual_cpu_burst) == 4) {
        pid_record_t pid_record = pid_record_new(
            pid, arrival_time, time_until_first_response, actual_cpu_burst);
        pid_records_append(&pid_records, pid_record);

        // Move to the next line
        while (*current != '\n' && *current != '\0') {
            current++;
        }
        if (*current == '\n') {
            current++;
        }
    }
    return pid_records;

}

int pid_records_sort_by(pid_records_t *self,
                        int (*compare)(const void *, const void *)) {
    qsort(self->pid_records, self->size, sizeof(pid_record_t), compare);
    return 0;
}

// ##### PID COMPLETION RECORD #####


uint32_t min_uint32_t(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}
uint32_t max_uint32_t(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

// int pid_completion_records_print(pid_results_t *self) {
//     pid_results_t pid_results = pid_results_from_pid_records(self);
//     pid_results_print(&pid_results);
//     return 0;
// }
//
int pid_completion_records_print(pid_records_t *self) {
    printf("seq = [");
    for (int i = 0; i < self->seq_pid_index; i++) {
        printf("%d", self->seq_pids[i]);
        if (i != self->seq_pid_index - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("+----+---------+--------+--------+--------+--------+-------------+---------------+\n");
    printf("| ID | Arrival | Burst  | Start  | Finish | Wait   | Turnaround  | Response Time |\n");
    printf("+----+---------+--------+--------+--------+--------+-------------+---------------+\n");
    for (int i = 0; i < self->size; i++) {
        printf("| %2d | %7d | %6d | %6d | %6d | %6d | %11d | %13d |\n",
               self->pid_records[i].pid, self->pid_records[i].arrival_time, // id, arrival
               self->pid_records[i].actual_cpu_burst, // burst
               self->pid_records[i].start_time, // start
               self->pid_records[i].completion_time, // finish
               self->pid_records[i].completion_time - self->pid_records[i].arrival_time,
               self->pid_records[i].first_response_time,
               self->pid_records[i].first_response_time - self->pid_records[i].arrival_time);
    }
    printf("+----+---------+--------+--------+--------+--------+-------------+---------------+\n");
    printf("Average waiting time: %.2f ms\n", average_waiting_time(self));
    printf("Average turnaround time: %.2f ms\n", average_turnaround_time(self));
    printf("Average response time: %.2f ms\n", average_response_time(self));
    return 0;
}
float average_waiting_time(pid_records_t *self) {
    float total_waiting_time = 0;
    for (int i = 0; i < self->size; i++) {
        total_waiting_time += self->pid_records[i].completion_time - self->pid_records[i].arrival_time;
    }
    return total_waiting_time / self->size;
}
float average_turnaround_time(pid_records_t *self) {
    float total_turnaround_time = 0;
    for (int i = 0; i < self->size; i++) {
        total_turnaround_time += self->pid_records[i].completion_time - self->pid_records[i].arrival_time;
    }
    return total_turnaround_time / self->size;
}
float average_response_time(pid_records_t *self) {
    float total_response_time = 0;
    for (int i = 0; i < self->size; i++) {
        total_response_time += self->pid_records[i].first_response_time - self->pid_records[i].arrival_time;
    }
    return total_response_time / self->size;
}

pid_result_t pid_result_new(uint32_t pid) {
    uint32_t pid_result = pid;
    uint32_t arrival_time = UINT32_MAX;
    uint32_t burst = 0;
    uint32_t start_time = UINT32_MAX;
    uint32_t finish_time = 0;
    uint32_t wait_time = 0;
    uint32_t first_response_time = UINT32_MAX;
    uint32_t turnaround = 0;
    uint32_t response_time = 0;
    pid_result_t pid_result_struct = {pid_result, arrival_time, burst, start_time, finish_time, wait_time, first_response_time, turnaround, response_time};
    return pid_result_struct;
}

pid_result_t pid_result_from_pid_record(pid_record_t *self) {
    uint32_t pid = self->pid;
    uint32_t arrival_time = self->arrival_time;
    uint32_t burst = self->actual_cpu_burst;
    uint32_t start_time = self->start_time;
    uint32_t finish_time = self->completion_time;
    uint32_t wait_time = start_time - arrival_time;
    uint32_t first_response_time = self->first_response_time;
    uint32_t turnaround = finish_time - arrival_time;
    uint32_t response_time = first_response_time - arrival_time;

    pid_result_t pid_result_struct = {pid, arrival_time, burst, start_time, finish_time, wait_time, first_response_time, turnaround, response_time};
    return pid_result_struct;
}


pid_results_t pid_results_new(uint32_t num_pids) {
    pid_result_t* pid_results_array = malloc(num_pids * sizeof(pid_result_t));
    pid_results_t pid_results = {num_pids, pid_results_array};
    for (int i = 0; i < num_pids; i++) {
        pid_results.pid_results[i] = pid_result_new(i+1);
    }
    return pid_results;
}

pid_results_t pid_results_from_pid_records(pid_records_t *self) {
    pid_results_t pid_results = pid_results_new(50);
    for (int i = 0; i < self->size; i++) {
        pid_result_t pid_result = pid_result_from_pid_record(&self->pid_records[i]);
        pid_results_merge_in(&pid_results, pid_result);
    }
    pid_results.seq_pids = self->seq_pids;
    pid_results.seq_pid_index = self->seq_pid_index;
    return pid_results;
}

int pid_results_merge_in(pid_results_t *self, pid_result_t pid_result) {
    uint32_t pid = pid_result.pid;
    uint32_t arrival = pid_result.arrival_time;
    uint32_t burst = pid_result.burst;
    uint32_t start = pid_result.start_time;
    uint32_t finish = pid_result.finish_time;
    // uint32_t wait = pid_result.wait_time;
    uint32_t wait = finish - arrival - burst;
    uint32_t first_response_time = pid_result.first_response_time;

    self->pid_results[pid-1].arrival_time = min_uint32_t(self->pid_results[pid-1].arrival_time, arrival);
    self->pid_results[pid-1].burst += burst;
    self->pid_results[pid-1].start_time = min_uint32_t(self->pid_results[pid-1].start_time, start);
    self->pid_results[pid-1].finish_time = max_uint32_t(self->pid_results[pid-1].finish_time, finish);
    self->pid_results[pid-1].wait_time += wait;
    self->pid_results[pid-1].first_response_time = min_uint32_t(self->pid_results[pid-1].first_response_time, first_response_time);

    uint32_t turnaround = self->pid_results[pid-1].finish_time - self->pid_results[pid-1].arrival_time;
    uint32_t response_time = self->pid_results[pid-1].first_response_time - self->pid_results[pid-1].arrival_time;

    self->pid_results[pid-1].turnaround = turnaround;
    self->pid_results[pid-1].response_time = response_time;
    return 0;
}

int pid_results_print(pid_results_t *self) {
    printf("seq = [");
    for (int i = 0; i < self->seq_pid_index; i++) {
        printf("%d", self->seq_pids[i]);
        if (i != self->seq_pid_index - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    printf("+----+---------+--------+--------+--------+---------+-------------+---------------+\n");
    printf("| ID | Arrival | Burst  | Start  | Finish | Wait    | Turnaround  | Response Time |\n");
    printf("+----+---------+--------+--------+--------+---------+-------------+---------------+\n");
    for (int i = 0; i < self->num_pids; i++) {

        uint32_t pid = self->pid_results[i].pid;
        uint32_t arrival = self->pid_results[i].arrival_time;
        uint32_t burst = self->pid_results[i].burst;
        uint32_t start = self->pid_results[i].start_time;
        uint32_t finish = self->pid_results[i].finish_time;
        uint32_t wait = self->pid_results[i].wait_time;
        uint32_t first_response_time = self->pid_results[i].first_response_time;
        uint32_t turnaround = self->pid_results[i].turnaround;
        uint32_t response_time = self->pid_results[i].response_time;
        if (arrival == UINT32_MAX) {
            continue;
        }
        printf("| %2d | %7d | %6d | %6d | %6d | %7d | %11d | %13d |\n", pid, arrival, burst, start, finish, wait, turnaround, response_time);
    }

    uint32_t total_wait = 0;
    uint32_t total_turnaround = 0;
    uint32_t total_response = 0;

    for (int i = 0; i < self->num_pids; i++) {
        if (self->pid_results[i].arrival_time == UINT32_MAX) {
            continue;
        }
        total_wait += self->pid_results[i].wait_time;
        total_turnaround += self->pid_results[i].finish_time - self->pid_results[i].arrival_time;
        total_response += self->pid_results[i].first_response_time - self->pid_results[i].arrival_time;
    }

    double average_wait = (double) total_wait / (double) self->num_pids;
    double average_turnaround = (double) total_turnaround / (double) self->num_pids;
    double average_response = (double) total_response / (double) self->num_pids;

    printf("+----+---------+--------+--------+--------+---------+-------------+---------------+\n");
    printf("Average waiting time: %.2f ms\n", average_wait);
    printf("Average turnaround time: %.2f ms\n", average_turnaround);
    printf("Average response time: %.2f ms\n", average_response);
    return 0;
}

int pid_results_debug_print(pid_results_t *self) {
    printf("seq = [");
    for (int i = 0; i < self->seq_pid_index; i++) {
        printf("%d", self->seq_pids[i]);
        if (i != self->seq_pid_index - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    printf("+----+---------+--------+--------+--------+---------+---------------------+-------------+---------------+\n");
    printf("| ID | Arrival | Burst  | Start  | Finish | Wait    | First Response Time | Turnaround  | Response Time |\n");
    printf("+----+---------+--------+--------+--------+---------+---------------------+-------------+---------------+\n");
    for (int i = 0; i < self->num_pids; i++) {

        uint32_t pid = self->pid_results[i].pid;
        uint32_t arrival = self->pid_results[i].arrival_time;
        uint32_t burst = self->pid_results[i].burst;
        uint32_t start = self->pid_results[i].start_time;
        uint32_t finish = self->pid_results[i].finish_time;
        uint32_t wait = self->pid_results[i].wait_time;
        uint32_t first_response_time = self->pid_results[i].first_response_time;
        uint32_t turnaround = self->pid_results[i].turnaround;
        uint32_t response_time = self->pid_results[i].response_time;
        if (arrival == UINT32_MAX) {
            continue;
        }
        printf("| %2d | %7d | %6d | %6d | %6d | %7d | %19d | %11d | %13d |\n", pid, arrival, burst, start, finish, wait, first_response_time, turnaround, response_time);
    }

    uint32_t total_wait = 0;
    uint32_t total_turnaround = 0;
    uint32_t total_response = 0;

    for (int i = 0; i < self->num_pids; i++) {
        if (self->pid_results[i].arrival_time == UINT32_MAX) {
            continue;
        }
        total_wait += self->pid_results[i].wait_time;
        total_turnaround += self->pid_results[i].finish_time - self->pid_results[i].arrival_time;
        total_response += self->pid_results[i].first_response_time - self->pid_results[i].arrival_time;
    }

    double average_wait = (double) total_wait / (double) self->num_pids;
    double average_turnaround = (double) total_turnaround / (double) self->num_pids;
    double average_response = (double) total_response / (double) self->num_pids;

    printf("+----+---------+--------+--------+--------+---------+---------------------+-------------+---------------+\n");
    printf("Average waiting time: %.2f ms\n", average_wait);
    printf("Average turnaround time: %.2f ms\n", average_turnaround);
    printf("Average response time: %.2f ms\n", average_response);
    return 0;
}
