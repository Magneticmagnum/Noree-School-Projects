/*
 * CSci4061 F2010 Assignment 1
 * login: norma272
 * Linux
 * date: 09/28/10
 * name: Joe Houlton, Chaichana Noree, Brian Norman
 * id: 3688484, , 4332223
 */

#include "node.h"
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_NODES 42
#define MAX_BUFFER 1024

int main(int argc, char** argv) {

	node_t nodes[MAX_NODES];
	char buffer[MAX_BUFFER];

	if (argc != 2) {
		// print usage
		return 0;
	}

	FILE* file = fopen(argv[1], "r");

	int i = 0;
	while (i < MAX_NODES && fgets(buffer, MAX_BUFFER - 1, file) != NULL) {
		char* token = strtok(buffer, ":");
		strcpy(token, nodes[i].prog);

		token = strtok(buffer, NULL);
		if (strcmp(token, "none") == 0) {
			nodes[i].num_children = 0;
		} else {
			// get children
		}

		token = strtok(buffer, NULL);
		strcpy(token, nodes[i].input);

		token = strtok(buffer, NULL);
		strcpy(token, nodes[i].output);

		nodes[i].status = INELIGIBLE;

		i++;
	}

	int nNodes = i;
	nodes[0].status = READY;

	int isReadyNode = 1;
	while (isReadyNode) {
		isReadyNode = 0;
		for (i = 0; i < nNodes; i++) {
			if (nodes[i].status == READY) {
				nodes[i].status = RUNNING;
				isReadyNode = 1;

				// RUN NODE
			}
		}
		for (i = 0; i < nNodes; i++) {
			if (nodes[i].status == RUNNING) {
				// wait for node
				nodes[i].status = FINISHED;
				for (int c = 0; c < nodes[i].num_children; c++) {
					nodes[nodes[i].children[c]].status = READY;
				}
			}
		}
	}
}
