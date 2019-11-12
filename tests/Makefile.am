TESTS = check_io
check_PROGRAMS = check_io
check_io_SOURCES = check_io.c $(srcdir)/check_io.h
check_io_CFLAGS = @CHECK_CFLAGS@ -Wl,-rpath=/usr/local/lib
check_io_LDADD = $(top_builddir)/src/libspi_ac483.a @CHECK_LIBS@
