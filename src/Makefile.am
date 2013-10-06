if HAVE_GLES
EGL_LIBS = -L/opt/vc/lib -lGLESv2
endif

AM_CFLAGS += \
	    $(EGL_CFLAGS) \
	    $(VG_CFLAGS)

AM_CPPFLAGS += -DSYSTEM_CONFIG_FILE_LOCATION='"$(sysconfdir)/robocakes.conf"'

AM_LDFLAGS += \
	     $(EGL_LIBS) \
	     $(VG_LIBS)

#DEBUG = -g

bin_PROGRAMS = robocakes

robocakes_SOURCES = \
		    tokenizer.c \
		    client.c \
		    conf.c \
		    server.c \
		    util.c \
		    sim_lib.c \
		    tcp_client_lib.c \
		    tcp_server_lib.c \
		    tcp_lib.c \
		    thread_lib.c \
		    timeloop_lib.c \
		    main.c

if HAVE_GLES
robocakes_SOURCES += \
		    display.c
endif
