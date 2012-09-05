LIBDIR=../lib
INC=../sql_db
CFLAGS=-Wall -I. -I$(INC)

install:	keval_test $(LIBDIR)/libkeyval.so

$(LIBDIR)/libkeyval.so: libkeyval.so
		mkdir -p $(LIBDIR)
		mv -f libkeyval.so libkeyval.so.tmp
		mv -f libkeyval.so.tmp $(LIBDIR)/libkeyval.so

keval_test:	test.o $(LIBDIR)/libkeyval.so
		gcc test.o -o keval_test -L $(LIBDIR) -lsqlite3 -lsqldb -lkeyval

libkeyval.so:	keyval.o
			gcc -shared -Wl,-soname,libkeyval.so -o libkeyval.so keyval.o -lc

keyval.o:	keyval.c
			gcc -fPIC $(CFLAGS) -c keyval.c
	
test.o:	test.c
		gcc $(CFLAGS) -c test.c
	
clean:
	rm -f *.o *.so keval_test
