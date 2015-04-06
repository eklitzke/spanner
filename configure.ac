AC_PREREQ([2.65])
AC_INIT([spanner], [0.1], [evan@eklitzke.org])
AC_USE_SYSTEM_EXTENSIONS
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL

AC_CHECK_LIB(cairo, main, [], [AC_MSG_ERROR([missing cairo])])
AC_CHECK_LIB(X11, main, [], [AC_MSG_ERROR([missing X11])])

# options
AC_ARG_ENABLE(debug,
	      [ --enable-debug     build with tons of debugging],
	      [if test x$enableval = xyes ; then
		  AC_DEFINE([DEBUG], 1, [debugging enabled])
	       fi]
	     )

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
