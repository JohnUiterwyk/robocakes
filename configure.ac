#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([robocakes], [0.1], [bugs@stooj.org])
AM_INIT_AUTOMAKE([-Wall -Werror gnu])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

dnl Declare variables
AC_SUBST(AM_CPPFLAGS,"")
AC_SUBST(AM_CFLAGS,"")
AC_SUBST(AM_LDFLAGS,"")

dnl This mechanism allows one to enable debug compilations...
AC_ARG_ENABLE(debug,
[  --enable-debug     enable debugging and disable optimization],
[  if test "$enable_debug" = "yes"; then
    DEBUG_CFLAGS="-g3 -DDEBUG"
   else
    DEBUG_CFLAGS="-O2 -DNDEBUG"
   fi
]
)
AM_CONDITIONAL(ENABLE_DEBUG, test "$enable_debug" = "yes")
AM_CFLAGS="$AM_CFLAGS $DEBUG_CFLAGS"
CFLAGS="$DEBUG_CFLAGS"

# Add crazy pi-specific locations
AM_CPPFLAGS="$AM_CPPFLAGS -I/opt/vc/include \
    -I/opt/vc/include/interface/vcos/pthreads"

AM_LDFLAGS="$AM_LDFLAGS -L/opt/vc/lib/ -lpthread"

# Pi-specific GLES check
AC_CHECK_HEADERS([/opt/vc/include/GLES/gl.h], [found_gles=yes],
                 [found_gles=no])

if test x$found_gles = xyes; then
    AC_DEFINE([HAVE_GLES], [1], [Define if GLES library is present])
    LIBS="$LIBS -lGLESv2"
fi

AM_CONDITIONAL(HAVE_GLES, test x$found_gles = xyes)

# FIXME: Replace `main' with a function in `-ljpeg':
#TODO Check if this is needed?
#AC_CHECK_LIB([jpeg], [main], [AC_DEFINE(HAVE_GLES)
    #LIBS="$LIBS -l"; break])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h limits.h netdb.h netinet/in.h stdint.h stdlib.h string.h strings.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor gethostname gettimeofday memmove memset pow socket sqrt strcasecmp strdup strerror strtol])

#AC_CONFIG_FILES([spec/Makefile
                 #src/Makefile])
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
