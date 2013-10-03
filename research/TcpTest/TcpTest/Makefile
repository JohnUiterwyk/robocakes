CC = gcc
CFLAGS = -g -Wall -std=c99
LIBS = tcp_lib.o tcp_server_lib.o tcp_client_lib.o thread_lib.o InputUtil.o
all: chat test_tcp

chat: main.o $(LIBS)
	$(CC) -o chat main.o $(LIBS) $(CFLAGS)

test_tcp: test_tcp_client_server.o $(LIBS)
	$(CC) -o test_tcp test_tcp_client_server.o $(LIBS) $(CFLAGS)


clean:
	rm -f *.o
	rm -f test_tcp
    
.PHONY: clean