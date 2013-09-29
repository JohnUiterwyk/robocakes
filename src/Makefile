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
       InputUtil.c \
       UdpLib.c \
       SimLib.c \
       main.c

OBJS = $(SRCS:.c=.o)

TARGET = robocakes


all: $(TARGET) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $(DEBUG) -c $^

robocakes: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	@rm -v *.o $(TARGET)
