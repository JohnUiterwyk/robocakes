ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_CPPFLAGS += -I$(srcdir)/src $(GLIB_CFLAGS)
AM_CPPFLAGS += -DSYSTEM_CONFIG_FILE_LOCATION='"$(sysconfdir)/robocakes.conf"'

bin_PROGRAMS = src/robocakes

noinst_LIBRARIES =

src_robocakes_CFLAGS = $(AM_CFLAGS) $(ROBOCAKES_CFLAGS)
src_robocakes_CPPFLAGS = $(AM_CPPFLAGS)

if HAVE_GLES
EGL_LIBS = -L/opt/vc/lib -lGLESv2
endif

PTHREAD_LIBS = -lpthread

src_robocakes_LDADD = $(ROBOCAKES_LIBS) \
		      $(EGL_LIBS) \
		      $(PTHREAD_LIBS)

# Probably not needed, but what the heck?
#robocakes_headers = src/client.h \
#		    src/conf.h \
#		    src/server.h \
#		    src/sim_lib.h \
#		    src/tcp_client_lib.h \
#		    src/tcp_lib.h \
#		    src/tcp_server_lib.h \
#		    src/thread_lib.h \
#		    src/timeloop_lib.h \
#		    src/tokenizer.h \
#		    src/udp_lib.h \
#		    src/util.h

#if HAVE_GLES
#robocakes_headers += src/display.h
#endif

src_robocakes_SOURCES = \
		    src/tokenizer.c \
		    src/conf.c \
		    src/server.c \
		    src/client.c \
		    src/util.c \
		    src/sim_lib.c \
		    src/tcp_client_lib.c \
		    src/tcp_server_lib.c \
		    src/tcp_lib.c \
		    src/thread_lib.c \
		    src/timeloop_lib.c \
		    src/main.c

if HAVE_GLES
src_robocakes_SOURCES += display.c
endif

TESTS = 

noinst_PROGRAMS = \
		  test/test_sim_lib \
		  test/test_tcp
if HAVE_GLES
noinst_PROGRAMS += test/test_display

test_test_display_CFLAGS = 
test_test_display_CPPFLAGS = $(AM_CPPFLAGS) \
			$(GLIB_CFLAGS)
test_test_display_LDADD = $(ROBOCAKES_LIBS) \
		     $(GLIB_LIBS) \
	_	     $(EGL_LIBS) \
		     $(PTHREAD_LIBS)
test_test_display_SOURCES = test/test_display.c \
		       src/display.c \
		       src/sim_lib.c

endif

test_test_sim_lib_CFLAGS = 
test_test_sim_lib_CPPFLAGS = $(AM_CPPFLAGS) \
			$(GLIB_CFLAGS)
test_test_sim_lib_LDADD = $(ROBOCAKES_LIBS) \
		     $(GLIB_LIBS)
test_test_sim_lib_SOURCES = test/test_sim_lib.c \
		       src/sim_lib.c

test_test_tcp_CFLAGS =
test_test_tcp_CPPFLAGS = $(AM_CPPFLAGS) \
		    $(GLIB_CFLAGS)
test_test_tcp_LDADD = $(ROBOCAKES_LIBS) \
		 $(GLIB_LIBS) \
		 $(PTHREAD_LIBS)
test_test_tcp_SOURCES = test/test_tcp.c \
		   src/tcp_lib.c \
		   src/thread_lib.c \
		   src/tcp_client_lib.c \
		   src/tcp_server_lib.c


man_MANS = 
doc_DATA = AUTHORS COPYING NEWS README.md \
	   examples/robocakes_server.conf \
	   examples/robocakes_client.conf

EXTRA_DIST = 
