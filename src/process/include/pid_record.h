// pid_record.h
#ifndef PID_RECORD_H
#define PID_RECORD_H

#include <stddef.h>
#include <stdint.h>


// process#, arrival time, time until first response, actual CPU burst>
typedef struct pid_record_t {
    uint32_t pid;
    uint32_t arrival_time;
    uint32_t time_until_first_response;
    uint32_t actual_cpu_burst;
    // ######
    uint32_t start_time;
    uint8_t has_started;
    uint32_t running_cpu_burst;
    uint32_t running_time_until_first_response;
    uint32_t first_response_time;
    uint32_t added_to_queue;
    float* exp_time_remaining_chart;
    uint32_t last_preempted;
    uint32_t* current_time;

    // ###################
    uint32_t completion_time;


} pid_record_t;

pid_record_t pid_record_new(uint32_t pid, uint32_t arrival_time, uint32_t time_until_first_response, uint32_t actual_cpu_burst);
int pid_record_print(pid_record_t* self);
int pid_record_compare_arrival_time(const void * a, const void * b);

typedef struct pid_records_t {
    uint32_t size;
    uint32_t capacity;
    pid_record_t* pid_records;
    // ####
    uint32_t* seq_pids;
    uint32_t seq_pid_index;
} pid_records_t;

pid_records_t pid_records_new();
int pid_records_append(pid_records_t* self, pid_record_t pid_record);
int pid_records_print(const pid_records_t* self);
int pid_records_sort_by(pid_records_t* self, int (*compare)(const void *, const void *));

// creates from stdin file
pid_records_t create_pid_records();
pid_records_t create_pid_records_no_stdin(const char* input);

float average_waiting_time(pid_records_t *self);
float average_turnaround_time(pid_records_t *self);
float average_response_time(pid_records_t *self);

typedef struct pid_result_t {
    uint32_t pid;
    uint32_t arrival_time;
    uint32_t burst;
    uint32_t start_time;
    uint32_t finish_time;
    uint32_t wait_time;
    uint32_t first_response_time;
    uint32_t turnaround;
    uint32_t response_time;
} pid_result_t;

pid_result_t pid_result_new(uint32_t pid);

typedef struct pid_results_t {
    uint32_t num_pids;
    pid_result_t *pid_results;
    // ####
    uint32_t* seq_pids;
    uint32_t seq_pid_index;
} pid_results_t;

pid_results_t pid_results_new(uint32_t num_pids);
pid_results_t pid_results_from_pid_records(pid_records_t *self);

int pid_results_merge_in(pid_results_t *self, pid_result_t pid_result);
int pid_results_print(pid_results_t *self);
int pid_results_debug_print(pid_results_t *self);



#endif // PID_RECORD_H