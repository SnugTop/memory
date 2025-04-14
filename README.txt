Memory Project -- COSC 3020 -- Project 4
Cameron Daly, Anthony Duan

## Overview: 
This project simulates a virtual memory management system that translates logical address to phyiscal addresses using a page table
and a TLB (Translation Lookaside Buffer). It demonstrates demand paging by loading pages from backing store file (BACKING_STORE.bin)
and implements page replacement when phyiscal memory is limited. 

------------------------------------------------------------------------------------------------------------------------------------------

#Features implemented:
## Memory Paging
* This incorporates a TLB and page table
* We use bit manipulation to take the write bit, page number, and offset from the 32-bit numbers
* This is the first part we worked on

##Two modes: one with 128 frames and another with 256 frames
* There exists a boolean variable called "limit"
* When limit = true, then we will only fill up 128 frames
* Keeping track of the total, we will reset to the 1st frame upon passing the 128th, as per FIFO
* Upon beginning the program, a prompt will allow the user to select the option of limiting memory
* Thus, it will be possible to test both modes
* This is the second part we worked on; it was followed by testing

## Address Translation 
* Uses bit shifting and masking to get the page number and offset from the logical address. 

## TLB Lookup and FIFO Replacement 
* Uses 16-entry TLB to speed up address translation. 
* If the TLB does not contain the page the program will then go to the page table. 
* If the page is not there a page fault is triggered. 

## Demand Paging
* When a page fault occurs, a 256-byte page is read from BACKING_STORE.bin and loaded int the available frame in physical memory. 

## Page Replacement (Limited Memory Mode)
* Part 2, if physical memory is limited to 128 frames then the program usses a FIFO page - replacement strategy.
* Dirty pages are written back tot the backing store file before being replaced. 

## Statistics Reporting
* At the end of the program we report the total number of addressess processed, page fault rate, TLB hit rate, and number of dirty pages.

-------------------------------------------------------------------------------------------------------------------------------------------

## List of files:
* backing_store.c
    --> This file handles page faults, updating the page_table, replacing the "current" index in the TLB with the new page-frame pair, and increments the count of filled frames and the TLB index. 
    --> The next free frame variable is also incremented; it is used to determine where to place the frame in the page table, as if it uses 128 frames and 128 are filled that variable cannot be incremented.
    --> If there is a 128-frame limit and all frames are filled, we write the victim frame's contents to the backing store and take it out of the TLB.
* main.c
    --> We read in command-line arguments, including whether to impose the 128-frame limit, and initialize the page table. We then read in each line and output the statistics.
* memory.c
    --> We declare the tables and arrays, as well as various int variables to keep track of page faults, tlb hits, tlb index, etc.
    --> This file includes a function that takes the page number, etc, from the 32-bit numbers being passed in by passing it into some helper functions. 
    --> We then search the TLB; should it not be there we search in the page table, and should that not work we go to the page fault handler in backing_store.c
* memory.h 
    --> we declare global variables and constants. 
* utils.c 
    --> contains helper methods to get page number, offset, write bit, and to modify TLB

The files in this are all in a directory that corresponds to their path in linux.

------------------------------------------------------------------------------------------------------------------------------------------

## Walk-through of code:
1. In main.c, we open the file being passed in
2. In main.c, command-line response to "Enter mode (2 for limited physical memory, anything else for full memory):" determines whether frame limit is 128 frames i.e. if limit = true
3. We enter memory.c and translate each address, getting variables page_number, offset, and write_bit
4. We then use utils.c for get_page_number(), get_offset(), and get_write_bit()
5. If the page_number is not in the TLB, we do not increment tlb_hits, but we search in the page table, we handle the page fault via backing_store.c; either way, we end up with a new or used frame number
6. These frame numbers start at 0 (first) and go up to at least 127 (128th)
7. We use search_tlb() and look at the "valid" attribute of page_table[page_number] for the search
8. If we have filled up all the frames and have limited memory, we select our victim frame, write it back, then remove corresponding page table / TLB entries.
9. We set the new page table entry, and increment variables corresponding to which frame we place our page in, and how many frames are full.
10. When setting a new page table entry, we set page_table[page_number].frame_number to the incremental variable referring to a frame
11. We set the valid attribute of page_table[page_number] to true, and the dirty attribute to false
12. We update the TLB via utils.c and insert_tlb()
13. Back in memory.c, we increment framesFilled and page_faults.
14. If we've passed the 128th frame with a 128-frame limit, we revert that incremental variable to the first frame
15. We compute the physical_address
16. If our write_bit is 1, we increment physical_memory[physical_address], and set frameWrite[frame_number] and dirty attribute of page_table[page_number] to true
17. Returning to main.c, we increment the number of addresses, and compute the statistics.

## Code Excecution: 
* run "cp GOLD_BACKING_STORE.bin BACKING_STORE.bin" to reset BACKING_STORE.bin for testing purposes
* Makefile can be run from root directory
* Run Project --> "./bin/a.out addresses.txt
        * choose "1" for 256 frames 
        * choose "2" for 128 frames 
* Run Test --> " ./test/check_bin (hex of address to check_bin)


