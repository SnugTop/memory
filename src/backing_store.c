// backing_store.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"

int handle_page_fault(int page_number) {
    // Open the backing store in read/update mode.
    FILE *bs = fopen(BACKING_STORE, "r+b");
    if (!bs) {
        perror("Error opening BACKING_STORE.bin");
        exit(1);
    }
    
    // If physical memory is limited and full, perform page replacement.
    // (Here, we assume that in limited mode only 128 frames are available.)
    if (limit && framesFilled >= 128) {
        // Identify the victim frame using FIFO (next_free_frame indicates the next slot to replace).
        int victim_frame = next_free_frame;
        //remove one frame from framesFilled (will be added back at program's end)
        framesFilled--;
        
        // If the victim frame has been written to, write its contents back to the backing store.
        if (frameWrite[victim_frame]) {
            int victim_page = framesToPages[victim_frame];  // Get the page currently in the victim frame.
            // Seek to the location in the backing store for the victim page.
            fseek(bs, victim_page * FRAME_SIZE, SEEK_SET);
            // Write the frame back.
            fwrite(physical_memory + (victim_frame * FRAME_SIZE), sizeof(char), FRAME_SIZE, bs);
            // Mark the frame as clean.
            frameWrite[victim_frame] = false;
        }
        
        // Invalidate the victim page’s entry in the page table.
        {
            int victim_page = framesToPages[victim_frame];
            page_table[victim_page].valid = 0;
        }
        
        // Invalidate any corresponding TLB entry.
        for (int i = 0; i < TLB_SIZE; i++) {
            if (tlb[i].page_number == framesToPages[victim_frame]) {
                // Mark the TLB entry invalid (here we set page_number to -1).
                tlb[i].page_number = -1;
            }
        }
        
        // Now, the victim frame is available for the new page.
        // (Do not increment framesFilled because we are replacing an existing frame.)
    }
    
    // Seek to the required page in the backing store.
    fseek(bs, page_number * FRAME_SIZE, SEEK_SET);
    // Load the page into physical memory at the current next_free_frame.
    fread(physical_memory + (next_free_frame * FRAME_SIZE), sizeof(char), FRAME_SIZE, bs);
    fclose(bs);
    
    // Update the page table for the new page.
    page_table[page_number].frame_number = next_free_frame;
    page_table[page_number].valid = 1;
    page_table[page_number].dirty = 0;
    
    // Update the mapping for this frame so that we know which page is stored in it.
    framesToPages[next_free_frame] = page_number;
    
    // Save the allocated frame number.
    int allocated_frame = next_free_frame;
    
    // Advance next_free_frame circularly.
    // if limit is true and next_free_frame = 128, memory.c will reset it to 0
    next_free_frame++;

    // Advance next_free_frame circularly.
    // update framesFilled. In replacement mode (when framesFilled is already 128), we already decremented it
    // thus we can still increement it
    framesFilled++;
    
    return allocated_frame;
}
