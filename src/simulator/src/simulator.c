#include "simulator.h"
#include "pid_record.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// #### PROCESS QUEUE ####
int process_queue_new(pid_records_t pid_records,
                      int (*compare)(const void *, const void *)) {
    process_queue_t process_queue = {compare, pid_records};
    return 0;
}

int process_queue_add(process_queue_t *process_queue, pid_record_t process) {
    pid_records_append(&process_queue->processes_queue, process);
    pid_records_sort_by(&process_queue->processes_queue, process_queue->compare);

    return 0;
}

pid_record_t process_queue_pop(process_queue_t *process_queue) {
    pid_record_t process = process_queue->processes_queue.pid_records[0];
    for (int i = 0; i < process_queue->processes_queue.size - 1; i++) {
        process_queue->processes_queue.pid_records[i] =
                process_queue->processes_queue.pid_records[i + 1];
    }
    process_queue->processes_queue.size--;
    return process;
}

// #### SIMULATOR ####
simulator_t simulator_new(pid_records_t* pid_records,
                          int (*compare)(const void *, const void *), uint32_t time_quantum, float alpha) {
    uint32_t current_time = 0;
    int current_index = 0;
    uint8_t has_current_process = 0;
    pid_record_t current_process_option = {0, 0, 0, 0, 0};

    pid_records_t pid_records_in_order = *pid_records;
    pid_records_sort_by(&pid_records_in_order, &pid_record_compare_arrival_time);

    pid_records_t empty_pid_records = pid_records_new();
    process_queue_t process_queue = {compare, empty_pid_records};

    pid_records_t pid_completion_records =
            pid_records_new();
    uint32_t jobs_remaining = pid_records_in_order.size;
    uint32_t time_quantum_remaining = time_quantum;
    float* exp_time_remaining_estimate = malloc(51 * sizeof(float));

    uint32_t* seq_pids = malloc(pid_records->size * sizeof(uint32_t));
    uint32_t seq_pid_index = 0;

    for (int i = 0; i < 51; i++) {
        exp_time_remaining_estimate[i] = 10;
    }

    // for (int i = 0; i < pid_records_in_order.size; i++) {
    //     pid_records_in_order.pid_records[i].exp_time_remaining_chart = exp_time_remaining_estimate;
    // }

    simulator_t simulator = {
        time_quantum,
        current_time, current_index,
        has_current_process, current_process_option,
        pid_records_in_order, process_queue,
        pid_completion_records,
        jobs_remaining,
        time_quantum_remaining,
        exp_time_remaining_estimate,
        alpha,
        seq_pids,
        seq_pid_index,
    };
    for (int i = 0; i < pid_records_in_order.size; i++) {
        simulator.pid_records_in_order.pid_records[i].exp_time_remaining_chart = simulator.exp_time_remaining_estimate;
        simulator.pid_records_in_order.pid_records[i].current_time = &simulator.current_time;
    }
    return simulator;
}

// 1. if another process is ready to run, add it to the process queue
// 2. if using preemptive scheduling, check if the current process should be
// added back to the process queue
// 3. if no process is running, get the next process from the process queue
// 4. run the process for one time step
// 5. if the process is done, add it to the completion records
// 6. increment the current time

int simulator_time_step(simulator_t *simulator) {
    // 1. if another process is ready to run, add it to the process queue
    // using the current index to keep track of the next process to add
    while (simulator->current_index < simulator->pid_records_in_order.size &&
           simulator->pid_records_in_order.pid_records[simulator->current_index]
           .arrival_time == simulator->current_time) {
        simulator->pid_records_in_order
                          .pid_records[simulator->current_index].added_to_queue = simulator->current_time;
        process_queue_add(&simulator->process_queue,
                          simulator->pid_records_in_order
                          .pid_records[simulator->current_index]);
        simulator->current_index++;
    }

    // 2. if using preemptive scheduling, check if the current process should be
    // added back to the process queue
    if (simulator->time_quantum && simulator->has_current_process &&
        simulator->current_process_option.running_cpu_burst > 0 && simulator->time_quantum_remaining == 0) {
        simulator->current_process_option.added_to_queue = simulator->current_time;
        simulator->current_process_option.last_preempted = simulator->current_time;
        process_queue_add(&simulator->process_queue,
                          simulator->current_process_option);

        simulator->has_current_process = 0;
        simulator->time_quantum_remaining = simulator->time_quantum;
    }

    // 3. if no process is running, get the next process from the process queue
    if (!simulator->has_current_process) {
        if (simulator->process_queue.processes_queue.size > 0) {
            simulator->current_process_option =
                    process_queue_pop(&simulator->process_queue);
            simulator->has_current_process = 1;
            simulator->time_quantum_remaining = simulator->time_quantum;
        }
    }

    // 4. run the process for one time step
    simulator->current_process_option.running_cpu_burst--;
    simulator->time_quantum_remaining--;
    if (simulator->current_process_option.has_started == 0) {
        simulator->current_process_option.start_time = simulator->current_time;
        simulator->current_process_option.has_started = 1;
    }
    if (simulator->current_process_option.running_time_until_first_response > 0) {
        simulator->current_process_option.running_time_until_first_response--;
    }


    // 5. if the process responded, record the first response time
    if (simulator->has_current_process &&
        simulator->current_process_option.running_time_until_first_response == 0 && simulator->current_process_option.first_response_time == 0) {
        simulator->current_process_option.first_response_time = simulator->current_time + 1;
        // printf("Time: %d, Burst Left: %d, Response Time: %d\n",
        //        simulator->current_time + 1, simulator->current_process_option.running_cpu_burst, simulator->current_process_option.first_response_time);
    }

    // 6. if the process is done, add it to the completion records
    if (simulator->has_current_process &&
        simulator->current_process_option.running_cpu_burst == 0) {
        simulator->current_process_option.completion_time = simulator->current_time + 1;
        simulator->has_current_process = 0;
        simulator->jobs_remaining--;

        simulator->seq_pids[simulator->seq_pid_index] = simulator->current_process_option.pid;
        simulator->seq_pid_index++;

        // tau_n+1 = alpha * t_n + (1 - alpha) * tau_n


        // tau_n = simulator->exp_time_remaining_estimate
        // t_n = simulator->current_process_option.actual_cpu_burst
        simulator->current_process_option.exp_time_remaining_chart[simulator->current_process_option.pid] =
                simulator->alpha * (float) simulator->current_process_option.actual_cpu_burst +
                (1 - simulator->alpha) * simulator->current_process_option.exp_time_remaining_chart[simulator->current_process_option.pid];

        pid_records_append(&simulator->pid_completion_records,
                           simulator->current_process_option);
    }

    // 7. increment the current time
    simulator->current_time++;
    return 0;
}

pid_records_t *simulator_run(simulator_t *simulator) {
    // this while loop will cause problems if
    while (simulator->jobs_remaining > 0) {
        simulator_time_step(simulator);
    }
    simulator->pid_completion_records.seq_pids = simulator->seq_pids;
    simulator->pid_completion_records.seq_pid_index = simulator->seq_pid_index;
    return &simulator->pid_completion_records;
}
