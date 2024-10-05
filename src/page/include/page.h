// page.h
#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>
#include <stdint.h>

// process#, arrival time, time until first response, actual CPU burst>
typedef struct page_record_t {
    uint32_t page_number;
    uint8_t dirty;

    // Assigned characteristics
    uint32_t arrival_time;
    uint32_t last_access_time;
    uint8_t second_chance;
} page_record_t;

page_record_t page_record_new(uint32_t page_number, uint8_t dirty);
int page_record_print(page_record_t *self);

// ##### PAGE RECORDS CONTAINER #####

typedef struct page_records_t {
    size_t size;
    size_t capacity;
    page_record_t *page_records;
    // sim out
    uint32_t page_faults;
    uint32_t write_backs;
} page_records_t;

page_records_t page_records_new();
int page_records_append(page_records_t *self, const page_record_t page_record);

int page_records_print(const page_records_t *self);
page_records_t create_page_records();
page_records_t create_page_records_no_stdin(const char* input);

int page_records_sort_by(page_records_t *self,
                        int (*compare)(const void *, const void *));

typedef struct page_result_t {
    page_record_t page_record;
} page_result_t;

typedef struct page_results_t {
    int size;
    page_records_t page_records;
} page_results_t;

page_results_t page_results_from_page_records(page_records_t *page_records);
int page_results_merge(page_results_t *self, page_result_t other);
int page_results_print(const page_results_t *self);

#endif // PAGE_H