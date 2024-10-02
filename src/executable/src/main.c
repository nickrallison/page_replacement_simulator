
#include "page.h"
#include "scheduler.h"

int main(int argc, char *argv[]) {
    page_results_t page_results = main_runner(argc, argv);
    page_results_print(&page_results);
    return 0;
}