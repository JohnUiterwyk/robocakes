CC = gcc
CFLAGS = -g -Wall -std=c99
LIBS = main.o tcp_lib.o thread_lib.o InputUtil.o
all: test_tcp

test_tcp: $(LIBS)
	$(CC) -o test_tcp $(LIBS) $(CFLAGS)


clean:
	rm -f *.o
	rm -f test_tcp
    
.PHONY: clean