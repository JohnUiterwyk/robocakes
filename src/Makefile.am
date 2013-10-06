AM_CFLAGS = \
	    $(EGL_CFLAGS) \
	    $(VG_CFLAGS)

AM_LDFLAGS = \
	     $(EGL_LIBS) \
	     $(VG_LIBS)

#DEBUG = -g

bin_PROGRAMS = robocakes

robocakes_SOURCES = \
		    tokenizer.c \
		    conf.c \
		    server.c \
		    util.c \
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
