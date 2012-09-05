LIBDIR=../lib
INC=.
CFLAGS=-Wall -I$(INC)

install:	sql_db_test $(LIBDIR)/libsqldb.so

$(LIBDIR)/libsqldb.so: libsqldb.so
		mkdir -p $(LIBDIR)
		mv -f libsqldb.so libsqldb.so.tmp
		mv -f libsqldb.so.tmp $(LIBDIR)/libsqldb.so

sql_db_test:	test.o $(LIBDIR)/libsqldb.so
		gcc test.o -o sql_db_test -L $(LIBDIR) -lsqlite3 -lsqldb

libsqldb.so:	sql_db.o
		gcc -shared -Wl,-soname,libsqldb.so -o libsqldb.so sql_db.o -lc

sql_db.o:	sql_db.c
		gcc -fPIC $(CFLAGS) -c sql_db.c
	
test.o:	test.c
		gcc -c test.c
	
clean:
	rm -f *.o *.so sql_db_test
