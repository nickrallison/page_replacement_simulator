
#include <stdio.h>

#include "page.h"
#include "simulator.h"
#include "scheduler.h"

int main(int argc, char *argv[]) {
    simulator_stats_t stats = main_runner(argc, argv);
    printf("Page Faults: %d\n", stats.page_faults);
    printf("Write Backs: %d\n", stats.write_backs);
    return 0;
}