# Project: File Transfer Program in C using Sockets

Files:
-----
filetransfer.h  : the header file that defines PDU struct and two methods
                  for sending and receiving a file to/from a socket

filetransfer.c  : the implementation file for header filetransfer.h

client.h        : the header file for client program
client.c        : the implementation file for client.h

server.h        : the header file for server program
server.c        : the implementation file for server.h

Makefile        : the makefile for auto compilation of the code

README.txt      : this file

Compilation:
-----------
Open a terminal and from the project folder type the following command

$> make

to clean all generated object files and executables issue the following command

$> make clean


Run the Server:
--------------
From the project folder after compilation issue the following command

$> ./server [ PORT ]

where optional PORT is port number. if omitted server runs on default port of 9000


Run the Client:
--------------
From the project folder after compilation issue the following command

$> ./client SERVER PORT

where SERVER is server hostname of ip address and PORT is port number

e.g. ./client 127.0.0.1 9000
