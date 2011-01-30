CC = gcc
CFLAGS = -D_REENTRANT
LDFLAGS = -lpthread -lnsl

all: web_server_http file_server

web_server_http: server.o http-util.o
	${CC} ${CFLAGS} ${LDFLAGS} -o web_server_http server.o http-util.o
file_server: server.o file-util.o
	${CC} ${CFLAGS} ${LDFLAGS} -o file_server server.o file-util.o

server.o: server.c
http-util.o: http-util.c
file-util.o: file-util.c
clean:
	rm server.o web_server_http http-util.o file-util.o
