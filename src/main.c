#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>  // Needed for strcmp
#include "memory.h"

int main(int argc, char *argv[]) {
    // Check that the program was called with exactly one command-line argument
    // which should be the name of the file containing the logical addresses.
    if (argc != 2) {
        printf("Usage: ./a.out addresses.txt\n");
        return 1;
    }

    // Attempt to open the file with the list of logical addresses.
    // Open in read mode ("r").
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open input file");
        return 1;
    }

    // Initialize the page table.
    // For each of the PAGE_TABLE_SIZE entries, mark the entry as invalid (valid = 0)
    // and not dirty (dirty = 0). These values will later be updated as pages are loaded.
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        page_table[i].valid = 0;
        page_table[i].dirty = 0;
    }

    // Get the simulation mode input from the user.
    // The mode is entered as a string (up to 15 characters) and then compared.
    // Mode "2" indicates that we are using a limited physical memory scenario.
    // Any other entry is treated as full memory mode.
    char mode[16] = "";
    printf("Enter mode (2 for limited physical memory, anything else for full memory): ");
    scanf("%15s", mode);

    // Compare the user input to "2". If it equals "2", set the global variable 'limit' to true.
    // 'limit' will be used by the rest of the program to determine if a limited number of physical
    // frames should be used (e.g., 128 frames rather than all available 256).
    if (strcmp(mode, "2") == 0) {
        limit = true;
    } else {
        limit = false;
    }

    // Now begin reading the logical addresses from the file.
    // Each logical address is read from the file as an unsigned integer (32-bit).
    // For each logical address, call translate_address() to perform the address translation.
    // Also, increment the global total_addresses counter.
    uint32_t logical_address;
    while (fscanf(fp, "%u", &logical_address) == 1) {
        // translate_address returns the physical address corresponding to the logical address.
        // It also updates the TLB and page table and prints the translation result.
        translate_address(logical_address);
        total_addresses++;
    }

    // Close the file since we've finished processing all addresses.
    fclose(fp);

    // After processing all addresses, print overall statistics.
    // These include: total number of addresses processed, number of page faults, page fault rate,
    // TLB hit count and TLB hit rate.
    printf("\n--- Stats ---\n");
    printf("Total Addresses: %d\n", total_addresses);
    printf("Page Faults: %d\n", page_faults);
    printf("Page Fault Rate: %.2f%%\n", (page_faults * 100.0) / total_addresses);
    printf("TLB Hits: %d\n", tlb_hits);
    printf("TLB Hit Rate: %.2f%%\n", (tlb_hits * 100.0) / total_addresses);

    // Count the number of page table entries that remain marked as dirty.
    // This is an additional stat to show how many pages have been modified but not yet written back.
    int dirty_count = 0;
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if (page_table[i].dirty)
            dirty_count++;
    }
    printf("Dirty Pages: %d\n", dirty_count);

    //temporary code to test if different modes work.
    if (limit == true) {
        printf("YES LIMIT\n");
    }
    else {
        printf("NO LIMIT\n");
    }
    int frames = 0;
    for (int count = 0; count < 256; count++) {
        if (frameWrite[count]) {
            frames++;
        }
    }
    printf("%d\n", frames);
    printf("%d\n", framesFilled);
    //end temporary code

    // Return 0 indicating normal program termination.
    return 0;
}
