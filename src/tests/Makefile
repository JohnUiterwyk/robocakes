CC = gcc
CFLAGS = -g -Wall -std=c99
LIBS = ../config.o ../InputUtil.o ../SimLib.o ../TimeLoopLib.o ../tokenizer.o ../UdpLib.o ../util.o

all: test_server test_client

test_server: test_server.o $(LIBS)
	$(CC) -o test_server test_server.o $(LIBS) $(CFLAGS)

test_client: test_client.o $(LIBS)
	$(CC) -o test_client test_client.o $(LIBS) $(CFLAGS)

clean:
	rm -f *.o
	rm -f ../*.o
	rm -f test_client
	rm -f test_server
    
.PHONY: clean