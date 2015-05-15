#ifndef NODE_H_
#define NODE_H_

#include <unistd.h>

#define MAX_BUFFER 1024
#define MAX_CHILDREN 10

#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

typedef struct node {
	int id; // corresponds to line number in graph text file
	char prog[MAX_BUFFER]; // prog + arguments
	char input[MAX_BUFFER]; // filename
	char output[MAX_BUFFER]; // filename
	int children[MAX_CHILDREN]; // children IDs
	int num_children; // how many children this node has
	int status; // ineligible/ready/running/finished
	pid_t pid; // track it when it's running
} node_t;

#endif /* NODE_H_ */
