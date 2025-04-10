#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>  // Needed for strcmp
#include "memory.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./a.out addresses.txt\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open input file");
        return 1;
    }

    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_table[i].valid = 0;
        page_table[i].dirty = 0;
    }

    // Use a character array instead of an undeclared "string" type
    char mode[16] = "";
    printf("Enter mode (2 for limited physical memory, anything else for full memory): ");
    scanf("%15s", mode);

    // Use strcmp to compare strings
    if (strcmp(mode, "2") == 0) {
        limit = true;
    } else {
        limit = false;
    }

    uint32_t logical_address;
    while (fscanf(fp, "%u", &logical_address) == 1) {
        translate_address(logical_address);
        total_addresses++;
    }

    fclose(fp);

    printf("\n--- Stats ---\n");
    printf("Total Addresses: %d\n", total_addresses);
    printf("Page Faults: %d\n", page_faults);
    printf("Page Fault Rate: %.2f%%\n", (page_faults * 100.0) / total_addresses);
    printf("TLB Hits: %d\n", tlb_hits);
    printf("TLB Hit Rate: %.2f%%\n", (tlb_hits * 100.0) / total_addresses);

    int dirty_count = 0;
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if (page_table[i].dirty) dirty_count++;
    }
    printf("Dirty Pages: %d\n", dirty_count);

    return 0;
}
