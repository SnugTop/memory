//utils.c
#include <stdint.h>
#include "memory.h"

uint8_t get_page_number(uint32_t addr) {
    return (addr >> 8) & 0xFF;
}

uint8_t get_offset(uint32_t addr) {
    return addr & 0xFF;
}

uint8_t get_write_bit(uint32_t addr) {
    return (addr >> 16) & 0x1;
}

int search_tlb(int page_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page_number == page_number) {
            return tlb[i].frame_number;
        }
    }
    return -1;
}

void insert_tlb(int page_number, int frame_number) {
    tlb_index = tlb_index % TLB_SIZE; //this way we only do the operation once
    tlb[tlb_index].page_number = page_number;
    tlb[tlb_index].frame_number = frame_number;
    tlb_index++;
}
