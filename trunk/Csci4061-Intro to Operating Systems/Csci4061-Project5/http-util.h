#ifndef HTTP_UTIL_H_
#define HTTP_UTIL_H_

int init(int portNum);
int accept_connection();
int get_request(int fd, char *filename);
int return_error(int fd, char *buf);
int return_result(int fd, char *content_type, char *buf, int numbytes);
void freemakeargv(char ** argv);
int makeargv(const char *s, const char *delimiters, char***argvp);

#endif /* HTTP_UTIL_H_ */
