CC = gcc
CFLAGS = -g -Wall
TCPLIBS = ../tcp_lib.o ../thread_lib.o ../tcp_client_lib.o ../tcp_server_lib.o
INCLUDES += -I/opt/vc/include \
	    -I/opt/vc/include/interface/vcos/pthreads
LDFLAGS = -L/opt/vc/lib -lGLESv2
DISPLAYLIBS = ../sim_lib.o ../display.o
SIMLIBS = ../sim_lib.o

DEBUG = -g

all: test_sim_lib test_display test_tcp

test_sim_lib: test_sim_lib.o $(SIMLIBS)
	$(CC) $(LDFLAGS) -o test_sim_lib $(SIMLIBS)

test_tcp: test_tcp.o  $(TCPLIBS)
	$(CC) $(LDFLAGS) -o test_tcp $(TCPLIBS)

test_display: test_display.o $(DISPLAYLIBS)
	$(CC) $(LDFLAGS) -o test_display $(DISPLAYLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEBUG) -c $^

clean:
	rm -f *.o
	#rm -f ../*.o
	rm -f test_tcp
	rm -f test_client
	rm -f test_display

.PHONY: clean
