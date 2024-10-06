//
// Created by nickr on 2024-09-21.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "page.h"
#include "simulator.h"

int fcfs_comp(const void *first, const void *second);
int sjn_comp(const void *first, const void *second);
int rr_comp(const void *first, const void *second);
int priority_comp(const void *first, const void *second);
int srt_comp(const void *first, const void *second);

simulator_stats_t main_runner(int argc, char *argv[]);
simulator_stats_t main_runner_no_stdin(int argc, char *argv[], char* input, uint32_t capacity, uint32_t interval);

#endif //SCHEDULER_H
