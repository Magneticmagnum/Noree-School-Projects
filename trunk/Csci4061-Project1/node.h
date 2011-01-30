#ifndef NODE_H_
#define NODE_H_

#include <unistd.h>

#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

typedef struct node {
	int id; // corresponds to line number in graph text file
	char prog[1024]; // prog + arguments
	char input[1024]; // filename
	char output[1024]; // filename
	int children[10]; // children IDs
	int num_children; // how many children this node has
	int status; // ineligible/ready/running/finished
	pid_t pid; // track it when it's running
} node_t;

#endif /* NODE_H_ */
