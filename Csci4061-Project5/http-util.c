#include "http-util.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

//function prototypes
int socket_fd;

pthread_mutex_t socket_lock = PTHREAD_MUTEX_INITIALIZER;

int init(int portNum) {
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	int enable = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &enable,
			sizeof(int));

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htons(INADDR_ANY);
	server.sin_port = htons(portNum);

	if (bind(socket_fd, (struct sockaddr*) &server, sizeof(server)) < 0) {
		perror("Error binding to socket: ");
		exit(1);
	}

	if (listen(socket_fd, 5) < 0) {
		perror("Error listening to connection: ");
		exit(1);
	}
	return 0;
}

int accept_connection() {
	struct sockaddr_in client_addr;
	unsigned int size = sizeof(struct sockaddr);
	pthread_mutex_lock(&socket_lock);
	int new_socket = accept(socket_fd, (struct sockaddr*) &client_addr, &size);
	pthread_mutex_unlock(&socket_lock);
	return new_socket;
}

int get_request(int fd, char *filename) {
	char buf[1024];
	char ** request;

	if (read(fd, buf, 1024) < 0) {
		perror("Error: Something wrong with requrest from client: \n");
		return -1;
	}
	if (makeargv(buf, " ", &request) < 0) {
		perror("Error: Something wrong parsing request: \n");
		return -1;
	}
	strncpy(filename, request[1], 1023);
	freemakeargv(request);
	return 0;
}

/**
 * Similarly, if something went wrong, you should write back to the socket descriptor:
 HTTP/1.1 404 Not Found
 Content-Type: text/html
 Content-Length: num-bytes-here
 Connection: Close
 (blank line)
 Error-message-here
 */
int return_error(int fd, char *buf) {
	FILE *client = fdopen(fd, "w");

	fprintf(client, "HTTP/1.1 404 Not Found\n");
	fprintf(client, "Content-Type: text/html\n");
	fprintf(client, "Content-Length: %d\n", (int) strlen(buf));
	fprintf(client, "Connection: Close\n\n");

	fflush(client);

	//the error message
	write(fd, buf, strlen(buf));

	close(fd);

	return 0;
}

/*
 * HTTP/1.1 200 OK
 Content-Type: content-type-here
 Content-Length: num-bytes-here
 Connection: Close
 (blank line)
 File-contents-here
 */
int return_result(int fd, char *content_type, char *buf, int numbytes) {
	FILE *client = fdopen(fd, "w");
	if (client == NULL) {
		perror("Error: Something wrong opening client's FD");
		return -1;
	}

	fprintf(client, "HTTP/1.1 200 OK\n");
	write(fd, "HTTP/1.1 200 OK", strlen("HTTP/1.1 200 OK"));

	fprintf(client, "Content-Type: %s\n", content_type);
	fprintf(client, "Content-Length: %d\n", numbytes);
	fprintf(client, "Connection: Close\n");
	fprintf(client, "\n");

	fflush(client);

	write(fd, buf, numbytes);

	fclose(client);

	printf("Wrote to fd %d", fd);

	return 0;
}

/*
 * Utility Functions taken from:
 * UNIX Systems Programming: Communication, Concurrency and Threads, 2nd Edition
 * by Kay Robbins, Steve Robbins
 * page 37
 */
int makeargv(const char *s, const char *delimiters, char ***argvp) {
	int error;
	int i;
	int numtokens;
	const char *snew;
	char *savePtr1, *savePtr2;
	char *t;

	if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
		errno = EINVAL;
		return -1;
	}
	*argvp = NULL;
	snew = s + strspn(s, delimiters); /* snew is real start of string */
	if ((t = malloc(strlen(snew) + 1)) == NULL)
		return -1;
	strcpy(t, snew);
	numtokens = 0;
	if (strtok_r(t, delimiters, &savePtr1) != NULL) /* count the number of tokens in s */
		for (numtokens = 1; strtok_r(NULL, delimiters, &savePtr1) != NULL; numtokens++)
			;

	/* create argument array for ptrs to the tokens */
	if ((*argvp = malloc((numtokens + 1) * sizeof(char *))) == NULL) {
		error = errno;
		free(t);
		errno = error;
		return -1;
	}
	/* insert pointers to tokens into the argument array */
	if (numtokens == 0)
		free(t);
	else {
		strcpy(t, snew);
		**argvp = strtok_r(t, delimiters, &savePtr2);
		for (i = 1; i < numtokens; i++)
			*((*argvp) + i) = strtok_r(NULL, delimiters, &savePtr2);
	}
	*((*argvp) + numtokens) = NULL; /* put in final NULL pointer */
	return numtokens;
}

void freemakeargv(char **argv) {
	if (argv == NULL)
		return;
	if (*argv != NULL)
		free(*argv);
	free(argv);
}
