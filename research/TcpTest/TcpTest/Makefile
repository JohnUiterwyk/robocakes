CC = gcc
CFLAGS = -g -Wall -std=c99
LIBS = tcp_lib.o tcp_server_lib.o tcp_client_lib.o thread_lib.o InputUtil.o
all: test_tcp

test_tcp: test_tcp_client_server.o $(LIBS)
	$(CC) -o test_tcp test_tcp_client_server.o $(LIBS) $(CFLAGS)


clean:
	rm -f *.o
	rm -f test_tcp
    
.PHONY: clean