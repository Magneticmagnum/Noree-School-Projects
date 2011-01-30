/*
 * util_http.c
 *
 *  Created on: Dec 11, 2010
 *      Author: Joe Houlton
 *      Author: Brian Norman
 *      Author: Magnum Noree
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define HTTP_VER "HTTP/1.1"
typedef struct {
	int status;
	char *method;
	char *path;
	char *httpVer;
	char *host;

} HttpRequest;

typedef struct {
	int status;
	char *method;
	char *contentType;
	int contentLength;
	char *connectionLine;
	char *fileContents;
} HttpResponse;

void HttpOkay(HttpResponse *response, char *contentType, char *contents,
		int contentLength) {
	response->status = 200;
	response->method = "OK";
	response->contentType = contentType;
	response->contentLength = contentLength;
	response->fileContents = contents;
}
void HttpNotFound(HttpResponse *response) {
	char *errMsg = "The file or location could not be found";
	response->status = 404;
	response->method = "Not found";
	response->contentType = "text/html";
	response->contentLength = strlen(errMsg);
	response->fileContents = errMsg;

}
// make sure path does not include two slashes or ..
int checkPath(char *path) {
	if (strstr(path, "..") >= 0 || strstr(path, "//")) {
		return -1;
	}
}
int main(int argc, char **argv) {
	int socketfd;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server;
	server.sin_family = AF_INET;

}
