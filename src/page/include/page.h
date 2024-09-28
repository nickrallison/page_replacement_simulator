// page.h
#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>
#include <stdint.h>

// process#, arrival time, time until first response, actual CPU burst>
typedef struct page_record_t {
    uint32_t page_number;
    uint8_t dirty;
} page_record_t;

page_record_t page_record_new(uint32_t page_number, uint8_t dirty);
int page_record_print(page_record_t *self);

// ##### PAGE RECORDS CONTAINER #####

typedef struct page_records_t {
    size_t size;
    size_t capacity;
    page_record_t *page_records;
} page_records_t;

page_records_t page_records_new();
int page_records_append(page_records_t *self, const page_record_t page_record);

int page_records_print(const page_records_t *self);
page_records_t create_page_records();
page_records_t create_page_records_no_stdin(const char* input);

int page_records_sort_by(page_records_t *self,
                        int (*compare)(const void *, const void *));

#endif // PAGE_H