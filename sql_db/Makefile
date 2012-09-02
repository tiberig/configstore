LIBDIR=.
INC=.
CFLAGS=-Wall -I$(INC)

all:	sql_db_test libsqldb.so

install: all
		mv -f libsqldb.so libsqldb.so.tmp
		mv -f libsqldb.so.tmp $(LIBDIR)/libsqldb.so

sql_db_test:	sql_db.o test.o
		gcc sql_db.o test.o -o sql_db_test -lsqlite3

libsqldb.so:	sql_db.o
		gcc -shared -Wl,-soname,libsqldb.so -o libsqldb.so sql_db.o -lc

sql_db.o:	sql_db.c
		gcc -fPIC $(CFLAGS) -c sql_db.c
	
test.o:	test.c
		gcc -c test.c
	
clean:
	rm -f *.o *.so sql_db_test
