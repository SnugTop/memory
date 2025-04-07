// include/memory.h
#ifndef MEMORY_H
#define MEMORY_H

#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16
#define FRAME_SIZE 256
#define PHYSICAL_MEMORY_SIZE 32768
#define BACKING_STORE "BACKING_STORE.bin"

typedef struct {
    int page_number;
    int frame_number;
} TLBEntry;

typedef struct {
    int frame_number;
    int valid;
    int dirty;
} PageTableEntry;

// Globals (extern so defined in memory.c)
extern char physical_memory[PHYSICAL_MEMORY_SIZE];
extern PageTableEntry page_table[PAGE_TABLE_SIZE];
extern TLBEntry tlb[TLB_SIZE];
extern int tlb_index;
extern int next_free_frame;
extern int total_addresses, page_faults, tlb_hits;

// Function declarations
uint8_t get_page_number(uint32_t addr);
uint8_t get_offset(uint32_t addr);
uint8_t get_write_bit(uint32_t addr);

int search_tlb(int page_number);
void insert_tlb(int page_number, int frame_number);
int handle_page_fault(int page_number);
int translate_address(uint32_t logical_address);

#endif
