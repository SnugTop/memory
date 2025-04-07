#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"

int handle_page_fault(int page_number) {
    FILE *bs = fopen(BACKING_STORE, "rb");
    if (!bs) {
        perror("Error opening BACKING_STORE.bin");
        exit(1);
    }

    bool TLBreplace = false;
    int locTLB = 0;
    if (framesFilled == 128) {
        if (frameWrite[next_free_frame]) {
            fwrite(physical_memory + (next_free_frame * FRAME_SIZE), sizeof(char), FRAME_SIZE, bs);
            frameWrite[next_free_frame] = false; //since now not necessarily written
        }
        page_table[framesToPages[next_free_frame]].valid = 0; //to show that it's invalid
        //and just in case in TLB...
        locTLB = search_tlb(framesToPages[next_free_frame]);
        if (locTLB != -1) {
            TLBreplace = true;
        }
    } //something written

    fseek(bs, page_number * FRAME_SIZE, SEEK_SET);
    fread(physical_memory + (next_free_frame * FRAME_SIZE), sizeof(char), FRAME_SIZE, bs);
    fclose(bs);

    if (TLBreplace) {
        tlb[locTLB].page_number = page_number;
    }

    page_table[page_number].frame_number = next_free_frame;
    
    page_table[page_number].valid = 1;
    page_table[page_number].dirty = 0;

    return next_free_frame++;
}
