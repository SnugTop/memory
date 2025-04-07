#include <stdio.h>
#include <stdint.h>
#include "memory.h"

char physical_memory[PHYSICAL_MEMORY_SIZE];
PageTableEntry page_table[PAGE_TABLE_SIZE];
TLBEntry tlb[TLB_SIZE];

int tlb_index = 0;
int next_free_frame = 0;

int total_addresses = 0;
int page_faults = 0;
int tlb_hits = 0;

int translate_address(uint32_t addr) {
    uint8_t page_number = get_page_number(addr);
    uint8_t offset = get_offset(addr);
    uint8_t write_bit = get_write_bit(addr);

    int frame_number = search_tlb(page_number);

    if (frame_number != -1) {
        tlb_hits++;
    } else {
        if (!page_table[page_number].valid) {
            frame_number = handle_page_fault(page_number);
            page_faults++;
        } else {
            frame_number = page_table[page_number].frame_number;
        }
        insert_tlb(page_number, frame_number);
    }

    int physical_address = (frame_number * FRAME_SIZE) + offset;
    signed char value = physical_memory[physical_address];

    printf("Logical: 0x%04X | Physical: 0x%04X | Value: %d", addr & 0xFFFF, physical_address, value);

    if (write_bit) {
        physical_memory[physical_address]++;
        page_table[page_number].dirty = 1;
        printf(" | [DIRTY]");
    }
    printf("\n");

    return physical_address;
}