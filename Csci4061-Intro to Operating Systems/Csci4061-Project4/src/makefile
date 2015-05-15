CC = gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread

all: web_server_http

web_server_http: server.o util-http.o
	${CC} ${LDFLAGS} -o web_server_http server.o util-http.o

server.o: server.c

clean:
	rm *.o web_server_http

