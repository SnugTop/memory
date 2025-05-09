//memory.c
#include <stdio.h>
#include <stdint.h>
#include "memory.h"

char physical_memory[PHYSICAL_MEMORY_SIZE];
PageTableEntry page_table[PAGE_TABLE_SIZE];
TLBEntry tlb[TLB_SIZE];

//to help with page replacement -- tells us how many are in
//we add frames in 0, 1, 2... 127 -- thus we don't need a specific queue
//we can use next_free_frame to tell us how to start
int framesFilled = 0; //how many have been placed into the table
bool frameWrite[256]; //if we've written to this frame
int framesToPages[256]; //reverse page table
//if "limit" is true, only first 128 will be used in both of these

int tlb_index = 0;
int next_free_frame = 0;

int total_addresses = 0;
int page_faults = 0;
int tlb_hits = 0;

bool limit = false; //default

int translate_address(uint32_t addr) {
    // Get the page number, offset, and write flag from the logical address.
    uint8_t page_number = get_page_number(addr);
    uint8_t offset = get_offset(addr);
    uint8_t write_bit = get_write_bit(addr);

    // Look for the page number in the TLB.
    int frame_number = search_tlb(page_number);
    if (frame_number != -1) {
        // TLB hit: count the hit.
        tlb_hits++;
    } else {
        // TLB miss: check the page table.
        if (!page_table[page_number].valid) {
            // Page fault: load the page from the backing store.
            frame_number = handle_page_fault(page_number);
            page_faults++;
            framesFilled++;
            // In limited-memory mode, wrap around after frame 127.
            if (limit && next_free_frame == 128) {
                next_free_frame = 0;
            }
        } else {
            // Page already in physical memory.
            frame_number = page_table[page_number].frame_number;
        }
        // Insert this page/frame pair into the TLB.
        insert_tlb(page_number, frame_number);
    }

    // Calculate the physical address.
    int physical_address = (frame_number * FRAME_SIZE) + offset;
    signed char value = physical_memory[physical_address];

    // Print the mapping result.
    printf("Logical: 0x%04X | Physical: 0x%04X | Value: %d", addr & 0xFFFF, physical_address, value);

    // If the write bit is set, update memory and mark the page as dirty.
    if (write_bit) {
        frameWrite[frame_number] = true;  // Mark frame as modified.
        framesToPages[frame_number] = page_number;
        physical_memory[physical_address]++;
        page_table[page_number].dirty = 1;
        printf(" | [DIRTY]");
    }
    printf("\n");

    return physical_address;
}
