Memory Project -- COSC 3020 -- Project 4
Cameron Daly, Anthony Duan

List of files:
backing_store.c -- 
main.c -- 
memory.c -- 
memory.h -- 
utils.c -- 

The files in this are all in a directory that corresponds to thier path in linux.

Features implemented:

##Memory Paging
* This incorporates a TLB and page table
* We use bit manipulation to take the write bit, page number, and offset from the 32-bit numbers

##Two modes: one with 128 frames and another with 256 frames
* There exists a boolean variable called "limit"
* When limit = true, then we will only fill up 128 frames
* Keeping track of the total, we will reset to the 1st frame upon passing the 128th, as per FIFO
* Upon beginning the program, a prompt will allow the user to select the option of limiting memory
* Thus, it will be possible to test both modes
