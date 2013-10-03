CC = gcc

INCLUDES += -I/opt/vc/include \
	    -I/opt/vc/include/interface/vcos/pthreads


LDFLAGS += -L/opt/vc/lib -lGLESv2

DEBUG = -g

SRCS = display.c \
       tokenizer.c \
       config.c \
       client.c \
       server.c \
       util.c \
       tcp_lib.c \
       tcp_client_lib.c \
       tcp_server_lib.c \
       thread_lib.c
       sim_lib.c \
       timeloop_lib.c \
       main.c

OBJS = $(SRCS:.c=.o)

TARGET = robocakes


all: $(TARGET) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEBUG) -c $^

robocakes: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

osx: main.c sim_lib.c util.c server.c client.c config.c tokenizer.c timeloop_lib.c tcp_client_lib.c tcp_server_lib.c tcp_lib.c thread_lib.c
	$(CC) -o robocakes main.c sim_lib.c util.c server.c client.c config.c timeloop_lib.c tokenizer.c tcp_client_lib.c tcp_server_lib.c tcp_lib.c thread_lib.c 
jbpi:main.c sim_lib.c util.c server.c client.c config.c tokenizer.c timeloop_lib.c tcp_client_lib.c tcp_server_lib.c tcp_lib.c thread_lib.c
	$(CC) -o robocakes main.c sim_lib.c util.c server.c client.c config.c timeloop_lib.c tokenizer.c tcp_client_lib.c tcp_server_lib.c tcp_lib.c thread_lib.c $(INCLUDES) -L/opt/vc/lib
clean:
	@rm -v *.o $(TARGET)

