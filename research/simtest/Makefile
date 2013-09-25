CC = gcc
CFLAGS = -Wall -pedantic -g -std=c99
DEP = SimLib.o UdpLib.o InputUtil.o main.o

all: sim

sim: $(DEP)
	$(CC) -o SimTest $(DEP) $(CFLAGS)

clean:
	rm -f *.o
	rm SimTest

.PHONY: clean sim
