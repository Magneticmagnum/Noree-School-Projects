/* CSci4061 F2010 Project 3
 * Chaichana Noree  ID: 4295362
 * Brian Norman    	ID: 4332223
 * Joe Houlton 	    ID: 3688484
 
 About
 ====================
 The purpose of this assignment is design a memory-efficient system.
 The performance of the dynamic memory manager shall be timed and
 should outperform the Unix heap management routine.  In addition,
 we call separate interrupt-driven programming functions
 
 How to compile
 ====================
 Extract all the files to a separate folder and navigate to the folder.
 From there you may build and run the following makefile commands:
 
 make - Builds everything (recommended)
 
 make main_malloc - Builds only the malloc memory allocater
 
 make main_mm - Builds only the memory manager allocater created by the team
 
 make packet - Builds the packet handler program
 
 
 How to run 
 ====================
 After building an executer from above, the programs can be run from 
 the shell as defined below:
 
 ./main_malloc	- Runs and tests the time for the Unix malloc system call
 
 ./main_mm 		- Runs and tests the time for the team's memory manager.
 
 ./packet 		- Runs and tests the signal handler
 