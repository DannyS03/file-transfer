all: client server

filetransfer.o: filetransfer.h filetransfer.c
	gcc -g -c filetransfer.c -o filetransfer.o

server: server.h server.c filetransfer.o
	gcc server.c filetransfer.o -o server

client: client.c client.h filetransfer.o
	gcc client.c filetransfer.o -o client

clean:
	rm -f *.o client server
