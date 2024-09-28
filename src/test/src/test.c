
#include <stdio.h>

#include "pid_record.h"
#include "scheduler.h"


char* test_1_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n";
char* test_2_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n40,20,194,199\n46,23,39,133\n17,31,97,131\n14,37,35,75\n11,41,33,131\n39,43,2,8\n";


// typedef struct pid_result_t {
// uint32_t pid;
// uint32_t arrival_time;
// uint32_t burst;
// uint32_t start_time;
// uint32_t finish_time;
// uint32_t wait_time;
// uint32_t first_response_time;
// uint32_t turnaround;
// uint32_t response_time;
// }
// pid_result_t;

int fcfs_test_1(int argc, char *argv[], char *input) {
    pid_results_t pid_results = main_runner_no_stdin(argc, argv, input);

    uint32_t num_pids = 3;
    uint32_t pid_expected[] = {14, 24, 42};
    uint32_t arrival_time_expected[] = {16, 10, 0};
    uint32_t burst_expected[] = {158, 186, 78};
    uint32_t start_time_expected[] = {264, 42, 0};
    uint32_t finish_time_expected[] = {422, 228, 264};
    uint32_t wait_time_expected[] = {248, 32, 213};
    uint32_t first_response_time_expected[] = {416, 142, 15};
    uint32_t turnaround_expected[] = {406, 218, 264};
    uint32_t response_time_expected[] = {400, 132, 15};

    // for pid in pid_results compare to expected
    for (int index = 0; index < num_pids; index++) {
        uint32_t pid = pid_expected[index];
        pid_result_t pid_result = pid_results.pid_results[pid-1];

        uint32_t arrival_time = arrival_time_expected[index];
        uint32_t burst = burst_expected[index];
        uint32_t start_time = start_time_expected[index];
        uint32_t finish_time = finish_time_expected[index];
        uint32_t wait_time = wait_time_expected[index];
        uint32_t first_response_time = first_response_time_expected[index];
        uint32_t turnaround = turnaround_expected[index];
        uint32_t response_time = response_time_expected[index];

        if (pid_result.pid != pid) {
            printf("pid_result.pid: %d != pid: %d\n", pid_result.pid, pid);
            return 1;
        }
        if (pid_result.arrival_time != arrival_time) {
            printf("pid_result.arrival_time: %d != arrival_time: %d\n", pid_result.arrival_time, arrival_time);
            return 1;
        }
        if (pid_result.burst != burst) {
            printf("pid_result.burst: %d != burst: %d\n", pid_result.burst, burst);
            return 1;
        }
        if (pid_result.start_time != start_time) {
            printf("pid_result.start_time: %d != start_time: %d\n", pid_result.start_time, start_time);
            return 1;
        }
        if (pid_result.finish_time != finish_time) {
            printf("pid_result.finish_time: %d != finish_time: %d\n", pid_result.finish_time, finish_time);
            return 1;
        }
        if (pid_result.wait_time != wait_time) {
            printf("pid_result.wait_time: %d != wait_time: %d\n", pid_result.wait_time, wait_time);
            return 1;
        }
        if (pid_result.first_response_time != first_response_time) {
            printf("pid_result.first_response_time: %d != first_response_time: %d\n", pid_result.first_response_time, first_response_time);
            return 1;
        }
        if (pid_result.turnaround != turnaround) {
            printf("pid_result.turnaround: %d != turnaround: %d\n", pid_result.turnaround, turnaround);
            return 1;
        }
        if (pid_result.response_time != response_time) {
            printf("pid_result.response_time: %d != response_time: %d\n", pid_result.response_time, response_time);
            return 1;
        }

    }
    return 0;
}

int sjf_test_1(int argc, char *argv[], char *input) {
    pid_results_t pid_results = main_runner_no_stdin(argc, argv, input);

    uint32_t num_pids = 3;
    uint32_t pid_expected[] = {14, 24, 42};
    uint32_t arrival_time_expected[] = {16, 10, 0};
    uint32_t burst_expected[] = {158, 186, 78};
    uint32_t start_time_expected[] = {78, 236, 0};
    uint32_t finish_time_expected[] = {236, 422, 78};
    uint32_t wait_time_expected[] = {62, 226, 27};
    uint32_t first_response_time_expected[] = {230, 336, 15};
    uint32_t turnaround_expected[] = {220, 412, 78};
    uint32_t response_time_expected[] = {214, 326,  15};

    // for pid in pid_results compare to expected
    for (int index = 0; index < num_pids; index++) {
        uint32_t pid = pid_expected[index];
        pid_result_t pid_result = pid_results.pid_results[pid-1];

        uint32_t arrival_time = arrival_time_expected[index];
        uint32_t burst = burst_expected[index];
        uint32_t start_time = start_time_expected[index];
        uint32_t finish_time = finish_time_expected[index];
        uint32_t wait_time = wait_time_expected[index];
        uint32_t first_response_time = first_response_time_expected[index];
        uint32_t turnaround = turnaround_expected[index];
        uint32_t response_time = response_time_expected[index];

        if (pid_result.pid != pid) {
            printf("pid_result.pid: %d != pid: %d\n", pid_result.pid, pid);
            return 1;
        }
        if (pid_result.arrival_time != arrival_time) {
            printf("pid_result.arrival_time: %d != arrival_time: %d\n", pid_result.arrival_time, arrival_time);
            return 1;
        }
        if (pid_result.burst != burst) {
            printf("pid_result.burst: %d != burst: %d\n", pid_result.burst, burst);
            return 1;
        }
        if (pid_result.start_time != start_time) {
            printf("pid_result.start_time: %d != start_time: %d\n", pid_result.start_time, start_time);
            return 1;
        }
        if (pid_result.finish_time != finish_time) {
            printf("pid_result.finish_time: %d != finish_time: %d\n", pid_result.finish_time, finish_time);
            return 1;
        }
        if (pid_result.wait_time != wait_time) {
            printf("pid_result.wait_time: %d != wait_time: %d\n", pid_result.wait_time, wait_time);
            return 1;
        }
        if (pid_result.first_response_time != first_response_time) {
            printf("pid_result.first_response_time: %d != first_response_time: %d\n", pid_result.first_response_time, first_response_time);
            return 1;
        }
        if (pid_result.turnaround != turnaround) {
            printf("pid_result.turnaround: %d != turnaround: %d\n", pid_result.turnaround, turnaround);
            return 1;
        }
        if (pid_result.response_time != response_time) {
            printf("pid_result.response_time: %d != response_time: %d\n", pid_result.response_time, response_time);
            return 1;
        }

    }
    return 0;
}

int main() {

    // fcfs test
    char* fcfs_argv[] = {"test", "FCFS", "0"};
    int fcfs_result_1 = fcfs_test_1(2, fcfs_argv, test_1_input);
    if (fcfs_result_1 != 0) {
        printf("fcfs_test_1 failed\n");
        return 1;
    }
    printf("fcfs_test_1 passed\n");

    // sjf test
    char* sjf_argv[] = {"test", "SJF", "0"};
    int sjf_result_1 = sjf_test_1(2, sjf_argv, test_1_input);
    if (sjf_result_1 != 0) {
        printf("sjf_test_1 failed\n");
        return 1;
    }
    printf("sjf_test_1 passed\n");
}
