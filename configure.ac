AC_INIT([libspi-ac483-dev], [0.0.1], [bug-report@address])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_PROG_AR
AC_PROG_CC
AC_PROG_RANLIB
PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])
AC_ARG_ENABLE(gcov, [AS_HELP_STRING([--enable-gcov], [enable coverage test])])
AC_MSG_CHECKING([whether to enable gcov])
AS_IF([test "x${enable_gcov}" = "xyes" ], AC_MSG_RESULT([yes]), AC_MSG_RESULT([no]))
AM_CONDITIONAL([ENABLEGCOV], [test "x${enable_gcov}" = "xyes"])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
