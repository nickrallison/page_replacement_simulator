
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "simulator.h"
#include "scheduler.h"


// char* test_1_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n";
// char* test_2_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n40,20,194,199\n46,23,39,133\n17,31,97,131\n14,37,35,75\n11,41,33,131\n39,43,2,8\n";

// char* read_file_to_string(char* filename) {
//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         return NULL;
//     }
//     fseek(file, 0, SEEK_END);
//     long length = ftell(file);
//     fseek(file, 0, SEEK_SET);
//     char* buffer = malloc(length + 1);
//     fread(buffer, 1, length, file);
//     fclose(file);
//     buffer[length] = '\0';
//     return buffer;
// }

int fifo_test_1() {
    char* fifo_argv[] = {"test", "FIFO"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n";
    uint32_t capacity = 3;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {1, 1};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("FIFO test 1 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int fifo_test_2() {
    char* fifo_argv[] = {"test", "FIFO"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n4,1\n2,1\n1,1\n6,1\n4,1\n1,1\n6,1\n";
    uint32_t capacity = 3;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {6, 6};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("FIFO test 2 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int fifo_test_3() {
    char* fifo_argv[] = {"test", "FIFO"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n4,1\n2,1\n1,1\n6,1\n4,1\n1,1\n6,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {4, 4};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("FIFO test 3 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int fifo_test_4() {
    char* fifo_argv[] = {"test", "FIFO"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,0\n4,1\n2,0\n1,1\n6,0\n4,1\n1,1\n6,0\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {4, 2};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("FIFO test 4 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int fifo_test_5() {
    char* fifo_argv[] = {"test", "FIFO"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,0\n4,1\n2,0\n1,1\n6,0\n4,1\n1,1\n6,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {4, 3};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("FIFO test 5 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int opt_test_1() {
    char* fifo_argv[] = {"test", "OPT"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n";
    uint32_t capacity = 3;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {1, 1};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("OPT test 1 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int opt_test_2() {
    char* fifo_argv[] = {"test", "OPT"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n0,1\n6,1\n1,1\n0,1\n7,1\n4,1\n5,1\n6,1\n5,1\n4,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {6, 6};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("OPT test 2 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int opt_test_3() {
    char* fifo_argv[] = {"test", "OPT"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n0,1\n6,1\n1,1\n0,1\n7,1\n4,0\n5,0\n6,0\n5,0\n4,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {6, 5};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("OPT test 3 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int lru_test_1() {
    char* fifo_argv[] = {"test", "LRU"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {1, 1};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("LRU test 1 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int lru_test_2() {
    char* fifo_argv[] = {"test", "LRU"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n0,1\n6,1\n1,1\n0,1\n7,1\n4,0\n5,0\n6,0\n5,0\n4,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {7,5};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("LRU test 2 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int clk_test_1() {
    char* fifo_argv[] = {"test", "CLK"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n";
    uint32_t capacity = 4;
    uint32_t interval = 0;
    uint32_t clock_reg_size = 0;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {1,1};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("CLK test 1 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int clk_test_2() {
    char* fifo_argv[] = {"test", "CLK"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n10,1\n5,1\n4,1\n3,1\n5,1\n1,1\n5,1\n2,1\n5,1\n4,1\n2,1\n0,1\n6,1\n1,1\n0,1\n7,1\n";
    uint32_t capacity = 5;
    uint32_t interval = 4;
    uint32_t clock_reg_size = 2;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {9,9};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("CLK test 2 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int clk_test_3() {
    char* fifo_argv[] = {"test", "CLK"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n10,1\n5,1\n4,1\n3,1\n5,1\n1,1\n5,1\n2,1\n5,1\n4,1\n2,1\n0,0\n6,1\n1,1\n0,1\n7,1\n";
    uint32_t capacity = 5;
    uint32_t interval = 4;
    uint32_t clock_reg_size = 2;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {9,9};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("CLK test 3 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int clk_test_4() {
    char* fifo_argv[] = {"test", "CLK"};
    int fifo_argc = 2;
    char* fifo_input = "Page#,dirty?\n6,1\n10,1\n5,1\n4,1\n3,1\n5,1\n1,1\n5,1\n2,1\n5,1\n4,1\n2,1\n0,0\n6,1\n1,1\n0,0\n7,1\n";
    uint32_t capacity = 5;
    uint32_t interval = 4;
    uint32_t clock_reg_size = 2;
    simulator_stats_t actual = main_runner_no_stdin(fifo_argc, fifo_argv, fifo_input, capacity, interval, clock_reg_size);
    simulator_stats_t expected = {9,8};
    if (actual.page_faults != expected.page_faults || actual.write_backs != expected.write_backs) {
        printf("CLK test 4 failed\n");
        printf("Expected: %d page faults, %d write backs\n", expected.page_faults, expected.write_backs);
        printf("Actual: %d page faults, %d write backs\n\n", actual.page_faults, actual.write_backs);
        return 1;
    }
    return 0;
}

int main() {

    // int fifo_test_1_result = fifo_test_1();
    // int fifo_test_2_result = fifo_test_2();
    // int fifo_test_3_result = fifo_test_3();
    // int fifo_test_4_result = fifo_test_4();
    // int fifo_test_5_result = fifo_test_5();
    //
    // int opt_test_1_result = opt_test_1();
    // int opt_test_2_result = opt_test_2();
    // int opt_test_3_result = opt_test_3();
    //
    // int lru_test_1_result = lru_test_1();
    // int lru_test_2_result = lru_test_2();

    // int clk_test_1_result = clk_test_1();
    int clk_test_2_result = clk_test_2();
    // int clk_test_3_result = clk_test_3();
    // int clk_test_4_result = clk_test_4();

    return 0;

}
