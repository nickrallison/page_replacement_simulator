// simulator.h
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stddef.h>
#include <stdint.h>

#include "pid_record.h"


/// @brief A struct to store a queue of processes
typedef struct process_queue_t {
    // function pointer to compare two processes
    int (*compare)(const void *, const void *);

    // pid_records struct to store the processes
    pid_records_t processes_queue;
} process_queue_t;

int process_queue_new(pid_records_t pid_records, int (*compare)(const void *, const void *));

int process_queue_add(process_queue_t *process_queue, pid_record_t process);


/// @brief A struct to store the simulator
typedef struct simulator_t {
    uint32_t time_quantum;

    uint32_t current_time;
    int current_index;

    uint8_t has_current_process;
    pid_record_t current_process_option;

    pid_records_t pid_records_in_order;

    process_queue_t process_queue;

    pid_records_t pid_completion_records;
    uint32_t jobs_remaining;
    uint32_t time_quantum_remaining;

    float* exp_time_remaining_estimate;
    float alpha;

    uint32_t* seq_pids;
    uint32_t seq_pid_index;

} simulator_t;

simulator_t simulator_new(pid_records_t* pid_records,
                          int (*compare)(const void *, const void *), uint32_t time_quantum, float alpha);

pid_records_t *simulator_run(simulator_t *simulator);


#endif // SIMULATOR_H
