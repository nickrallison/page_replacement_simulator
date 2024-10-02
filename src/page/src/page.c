#include "page.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

page_record_t page_record_new(uint32_t page_number, uint8_t dirty) {
    return (page_record_t){page_number, dirty};
}

int page_record_print(page_record_t *self) {
    printf("Page Number: %d, Dirty: %d\n", self->page_number, self->dirty);
    return 0;
}

page_records_t page_records_new() {
    uint32_t size = 0;
    uint32_t capacity = 32;
    page_record_t *records_array = malloc(capacity * sizeof(page_record_t));
    page_records_t page_records = {size, capacity, records_array};
    return page_records;
}

int page_records_append(page_records_t *self, const page_record_t page_record) {
    if (self->size == self->capacity - 1) {
        // malloc an array with double the space
        self->capacity *= 2;
        page_record_t *new_buffer = malloc(self->capacity * sizeof(page_record_t));

        // copy the elements
        memcpy(new_buffer, self->page_records, self->size * sizeof(page_record_t));

        // free the old array
        free(self->page_records);

        // copy the new buffer to self->page_records
        self->page_records = new_buffer;
    }

    self->page_records[self->size] = page_record;
    self->size++;
    return 0;
}

int page_records_print(const page_records_t *self) {
    for (int i = 0; i < self->size; i++) {
        page_record_print(&self->page_records[i]);
    }
    return 0;
}

page_records_t create_page_records() {
    uint32_t page_number;
    uint8_t dirty;

    page_records_t page_records = page_records_new();

    // get rid of the first line
    char *first_line_char = malloc(sizeof(char));
    *first_line_char = '\0';
    while (*first_line_char != '\n') {
        *first_line_char = getchar();
    }

    // Reads stdin line by line and creates a page_record_t for each line
    while (scanf("%d,%hhd\n", &page_number, &dirty) != EOF) {
        page_record_t page_record = page_record_new(page_number, dirty);
        page_records_append(&page_records, page_record);
    }
    return page_records;
}

page_records_t create_page_records_no_stdin(const char* input) {
    uint32_t page_number;
    uint8_t dirty;

    page_records_t page_records = page_records_new();

    // get rid of the first line
    const char *current = input;
    while (*current != '\n' && *current != '\0') {
        current++;
    }
    if (*current == '\n') {
        current++;
    }


    // Reads stdin line by line and creates a page_record_t for each line
    while (sscanf(current, "%d,%hhd\n", &page_number, &dirty) == 2) {
        page_record_t page_record = page_record_new(
            page_number, dirty);
        page_records_append(&page_records, page_record);

        // Move to the next line
        while (*current != '\n' && *current != '\0') {
            current++;
        }
        if (*current == '\n') {
            current++;
        }
    }
    return page_records;

}

int page_records_sort_by(page_records_t *self,
                        int (*compare)(const void *, const void *)) {
    qsort(self->page_records, self->size, sizeof(page_record_t), compare);
    return 0;
}

