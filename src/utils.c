// utils.c
#include <stdint.h>
#include "memory.h"

// Extracts the page number from a 32-bit logical address.
// The page number is stored in bits 8 through 15.
uint8_t get_page_number(uint32_t addr) {
    return (addr >> 8) & 0xFF;
}

// Extracts the offset from a 32-bit logical address.
// The offset is stored in the lower 8 bits.
uint8_t get_offset(uint32_t addr) {
    return addr & 0xFF;
}

// Extracts the write flag (bit) from a 32-bit logical address.
// The write bit is stored at bit position 16.
uint8_t get_write_bit(uint32_t addr) {
    return (addr >> 16) & 0x1;
}

// Searches the TLB for a given page number.
// If the page is found, returns the corresponding frame number.
// If not found, returns -1 to indicate a TLB miss.
int search_tlb(int page_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        // Check if the current TLB entry has the page number.
        if (tlb[i].page_number == page_number) {
            return tlb[i].frame_number;
        }
    }
    return -1; 
}

// Inserts a page number and its associated frame number into the TLB.
// This implementation uses a FIFO replacement strategy.
// It cycles through the TLB entries using modulo arithmetic.
void insert_tlb(int page_number, int frame_number) {
    tlb_index = tlb_index % TLB_SIZE;  // Wrap around if index exceeds TLB size.
    tlb[tlb_index].page_number = page_number;  // Insert the new page number.
    tlb[tlb_index].frame_number = frame_number;  // Insert the corresponding frame number.
    tlb_index++;  // Move to the next TLB slot for future insertions.
}
