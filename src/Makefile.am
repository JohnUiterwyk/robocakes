AM_CFLAGS = \
	    $(EGL_CFLAGS) \
	    $(VG_CFLAGS)

AM_LDFLAGS = \
	     $(EGL_LIBS) \
	     $(VG_LIBS)

CFLAGS = --pedantic -std=c99 -O2

DEBUG = -g

bin_PROGRAMS = robocakes

robocakes_headers = \
		    conf.h \
		    server.h \
		    sim_lib.h \
		    tcp_client_lib.h \
		    tcp_lib.h \
		    tcp_server_lib.h \
		    thread_lib.h \
		    timeloop_lib.h \
		    tokenizer.h \
		    udp_lib.h \
		    util.h

if HAVE_GLES
robocakes_headers += \
		    client.h \
		    display.h 
endif

robocakes_SOURCES = \
		    $(robocakes_headers) \
		    tokenizer.c \
		    conf.c \
		    server.c \
		    util.c \
		    udp_lib.c \
		    sim_lib.c \
		    tcp_client_lib.c \
		    tcp_server_lib.c \
		    tcp_lib.c \
		    thread_lib.c \
		    timeloop_lib.c
		    main.c

if HAVE_GLES
robocakes_SOURCES += \
		    client.c \
		    display.c
endif
