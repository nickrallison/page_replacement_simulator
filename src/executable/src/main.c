
#include "page.h"
#include "scheduler.h"

int main(int argc, char *argv[]) {
    page_records_t page_records = main_runner(argc, argv);
    page_records_print(&page_records);
    return 0;
}