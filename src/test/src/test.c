
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "simulator.h"
#include "scheduler.h"


// char* test_1_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n";
// char* test_2_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n40,20,194,199\n46,23,39,133\n17,31,97,131\n14,37,35,75\n11,41,33,131\n39,43,2,8\n";

char* read_file_to_string(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';
    return buffer;
}

int fifo_test_1() {
    char* fifo_argv[] = {"test", "FIFO"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n";
    uint32_t capacity = 3;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t stats = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    printf("Frames: %.4d, Page Faults: %.6d, Write Backs: %.6d\n",
           capacity, stats.page_faults, stats.write_backs);
}


int main() {

    // fcfs test
    char* fifo_argv[] = {"test", "CLK"};
    int fifo_argc = 2;
    char* fifo_input = read_file_to_string("cpsca2input.csv");
    for (int i = 1; i < 100; i++) {
        uint32_t capacity = i;
        uint32_t interval = 10;
        uint32_t clock_reg_size = 64;
        simulator_stats_t stats = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
        // spaced out for readability
        // remove leading 0's
        printf("%d, %d, %d\n", capacity, stats.page_faults, stats.write_backs);
        // printf("Frames: %.4d, Page Faults: %.6d, Write Backs: %.6d\n",
        //        capacity, stats.page_faults, stats.write_backs);
    }

}
