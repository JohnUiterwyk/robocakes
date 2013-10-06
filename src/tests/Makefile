CC = gcc
CFLAGS = -g -Wall -std=c99 -posix
TCPLIBS = ../tcp_lib.o ../thread_lib.o ../tcp_client_lib.o ../tcp_server_lib.o

all: test_sim_lib test_tcp

test_sim_lib: test_sim_lib.o ../sim_lib.o 
	$(CC) -o test_sim_lib test_sim_lib.o ../sim_lib.o $(CFLAGS)

test_tcp: test_tcp.o  $(TCPLIBS)
	$(CC) -o test_tcp test_tcp.o $(TCPLIBS) $(CFLAGS) -I/opt/vc/include/interface/vcos/pthreads


clean:
	rm -f *.o
	rm -f ../*.o
	rm -f test_tcp
	rm -f test_client
    
.PHONY: clean