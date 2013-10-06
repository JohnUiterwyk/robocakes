AM_CFLAGS = \
	    $(EGL_CFLAGS) \
	    $(VG_CFLAGS)

AM_LDFLAGS = \
	     $(EGL_LIBS) \
	     $(VG_LIBS)

CFLAGS = --pedantic -Wall -std=c99 -02

DEBUG = -g

bin_PROGRAMS = robocakes

robocakes_SOURCES = display.c \
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
