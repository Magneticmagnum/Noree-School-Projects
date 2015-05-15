/*
 * CSci4061 F2010 Assignment 1
 * login: norma272
 * Linux
 * date: 09/28/10
 * name: Joe Houlton, Chaichana Noree, Brian Norman
 * id: 3688484, 4295362, 4332223
 *
 * Description:  This program is designed to execute a set of programs given by
 * an input graph file.  Entries in the graph file are numbered sequentially,
 * and follow the format given in the example graph file
 * <program name>:<child 1> <child 2> <etc... >:<input redirect>:<output redirect>
 *
 * Standard in and standard out are redirected to the given input and output files
 * using dup2, giving this program the ability to run a graph that specifies
 * a series of intermediate steps using redirected input and output to achieve
 * a final result.
 *
 * Building:  Included with this source code is a make file.  Simply use the 
 * target make all, and run the resultant csci4061-assign1 executable with the
 * graph file as an argument.
 */