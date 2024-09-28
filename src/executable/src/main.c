
#include "pid_record.h"
#include "scheduler.h"

int main(int argc, char *argv[]) {
    pid_results_t pid_results = main_runner(argc, argv);
    pid_results_print(&pid_results);
    return 0;
}