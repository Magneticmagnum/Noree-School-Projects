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
 */

#include "node.h"
#include "makeargv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <errno.h>

#define MAX_NODES 50
#define STDIN 0
#define STDOUT 1

int main(int argc, char** argv)
{
  int rungraph(node_t*, int);
  int buildgraph(FILE*, node_t*, int);

  node_t nodes[MAX_NODES];

  if (argc != 2) {
    printf("USEAGE: graphexec [graph file]\n");
    return 0;
  }

  FILE* file = fopen(argv[1], "r");
  if (file != NULL) {
    int size = buildgraph(file, nodes, MAX_NODES);
    fclose(file);
    rungraph(nodes, size);
  }
  else {
    printf("ERROR: Unable to open the graph file\n");
  }

  return 0;
}

/*
 * Fork and execvp a child from a given node in the graph.  Also redirects input
 * and output using dup2
 */
void forkexec(node_t* node)
{
  int pid = fork();
  if (pid == 0) {

    int fdin = open(node->input, O_RDONLY | O_CREAT, 0666);
    int fdout = open(node->output, O_RDWR | O_CREAT, 0777);
    if (fdin < 0) {
      perror("fdin could not be opened! ");
      exit(0);
    }
    if (fdout < 0) {
      perror("fdout could not be opened! ");
      exit(0);
    }
    dup2(fdin, STDIN);
    dup2(fdout, STDOUT);
    char **args;
    makeargv(node->prog, " ", &args);
    execvp(args[0], args);

    // error! exec didn't work
    freemakeargv(args);
    close(fdin);
    close(fdout);
    exit(0);
  }
  else if (pid < 0) {
    perror("Error.  Insufficient memory or reached limit of children nodes.");
  }
}

/*
 * Execute the graph, traversing it by parent levels only.  Once all the running
 * children on a given level have finished executing, the graph advances one
 * level and forkexecs the rest of the eligible children.
 */
int rungraph(node_t* nodes, int size)
{
  void forkexec(node_t*);

  int isReadyNode = 1;
  int i;
  while (isReadyNode) {
    isReadyNode = 0;
    for (i = 0; i < size; i++) {
      if (nodes[i].status == READY) {
        nodes[i].status = RUNNING;
        isReadyNode = 1;
        forkexec(&(nodes[i]));
      }
    }
    // execute nodes
    for (i = 0; i < size; i++) {
      if (nodes[i].status == RUNNING) {
        wait(&(nodes[i].status));
        nodes[i].status = FINISHED;
        int c;
        for (c = 0; c < nodes[i].num_children; c++) {
          nodes[nodes[i].children[c]].status = READY;
        }
      }
    }
  }
  return i;
}

/*
 * Construct the graph, setting all nodes to ineligible first.  Afterwards,
 * check to see which nodes are parents and set those to eligible.
 */
int buildgraph(FILE* graph, node_t* nodes, int size)
{
  char buffer[MAX_BUFFER];

  // build graph from graphfile
  int i = 0;
  while (i < size && fgets(buffer, MAX_BUFFER - 1, graph) != NULL) {
    node_t* n = nodes + i; // pointer to the ith node

    // get program and arguments
    char* token = strtok(buffer, ":");
    strcpy(n->prog, token);

    // get the children string, parsing must be done after other arguments
    // set num_children to flag if there are children, zero if there aren't
    char* children = strtok(NULL, ":");
    if (strcmp(children, "none") == 0) {
      n->num_children = 0;
    }
    else {
      n->num_children = -1;
    }

    // get the input file string
    token = strtok(NULL, ":");
    strcpy(n->input, token);

    // get the output file string
    token = strtok(NULL, ":");
    strcpy(n->output, token);
    int len = strlen(n->output);

    // remove line delimiters
    int j;
    for (j = 0; j < len; j++) {
      if (n->output[j] == '\n' || n->output[j] == '\r') {
        n->output[j] = '\0';
        break;
      }
    }

    // set the status to be ineligible to run
    n->status = INELIGIBLE;

    // finally, parse the children string and get the number of children
    if (n->num_children == -1) {
      token = strtok(children, " ");
      int j = 0;
      while (token != NULL) {
        n->children[j] = atoi(token);

        token = strtok(NULL, ":");
        j++;
      }
      n->num_children = j;
    }

    i++;
  }
  int nNodes = i;

  int firstChild = nNodes;
  i = 0;
  while (i < firstChild) {
    int j;
    node_t* n = nodes + i; // pointer to the ith node
    for (j = 0; j < n->num_children; j++) {
      if (n->children[j] < firstChild) {
        firstChild = n->children[j];
      }
    }
    n->status = READY;
    i++;
  }

  return nNodes;
}
