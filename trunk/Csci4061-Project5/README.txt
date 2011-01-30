/* CSci4061 F2010 Assignment 5
 * section: one_digit_number
 * login: hoult015
 * date: 12/16/2010
 * names: Brian Norman, Chaichana Noree, Joseph Houlton
 * ID:    4332223     , 4295362        , 3688484
 */

how to make:

make all: Compiles and links both web_server_http and file_server binaries
make web_server_http: makes only web_server_http binary
make file_server: makes only file_server

About
==================
The purpose of this assignment is to construct a multithreaded web server using
POSIX threads (pthreads) in C to learn about thread programming and 
synchronization methods, as well as to form a utility library for socket programming.  
The web server handles any tope of files such as 
HTML, GIF, JPEG, TXT, etc.  It will also handle a limited portion of the
HTTP web protocol.  The server will be composed of three different types of 
threads; dispatcher, worker.  The dispatcher thread repeadly
accepts an incoming connection, read in the request, and place the request
in queue for the worker thread.  The worker thread monitors the request queue, 
picks up requests form it and serves it to the client.