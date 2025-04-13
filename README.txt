Memory Project -- COSC 3020 -- Project 4
Cameron Daly, Anthony Duan

List of files:
backing_store.c
--> This file handles page faults, updating the page_table, replacing the "current" index in the TLB with the new page-frame pair, and increments the count of filled frames and the TLB index. The next free frame variable is also incremented; it is used to determine where to place the frame in the page table, as if it uses 128 frames and 128 are filled that variable cannot be incremented.
--> If there is a 128-frame limit and all frames are filled, we write the victim frame's contents to the backing store and take it out of the TLB.
main.c
--> We read in command-line arguments, including whether to impose the 128-frame limit, and initialize the page table. We then read in each line and output the statistics.
memory.c
--> We declare the tables and arrays, as well as various int variables to keep track of page faults, tlb hits, tlb index, etc.
--> This file includes a function that takes the page number, etc, from the 32-bit numbers being passed in by passing it into some helper functions. We then search the TLB; should it not be there we search in the page table, and should that not work we go to the page fault handler in backing_store.c
memory.h 
--> we declare global variables and constants
utils.c 
--> contains helper methods to get page number, offset, write bit, and to modify TLB

The files in this are all in a directory that corresponds to their path in linux.

Features implemented:

##Memory Paging
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
