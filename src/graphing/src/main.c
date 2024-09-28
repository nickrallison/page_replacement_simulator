
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "pid_record.h"
#include "scheduler.h"


char* test_1_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n";
char* test_2_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n40,20,194,199\n46,23,39,133\n17,31,97,131\n14,37,35,75\n11,41,33,131\n39,43,2,8\n";

char* read_file(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(file_size + 1);
    if (buffer == NULL) {
        printf("Error allocating buffer\n");
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    fclose(file);

    buffer[file_size] = 0;
    return buffer;
}



int main(int argc, char *argv[]) {
    char* input = read_file(argv[1]);
    int local_argc = 3;
    int rr_min = 1;
    int rr_max = 100;
    for (int i = rr_min; i <= rr_max; i++) {
        // int to string
        char str[12];
        sprintf(str, "%d", i);
        char* local_argv[] = {"./scheduler", "RR", str};
        pid_results_t pid_results = main_runner_no_stdin(local_argc, local_argv, input);
        pid_results_debug_print(&pid_results);
    }

    // char* local_argv[] = {"./scheduler", "RR", "10"};
    // pid_results_t pid_results = main_runner_no_stdin(local_argc, local_argv, input);

    printf("do thing with pid_results\n");
}
