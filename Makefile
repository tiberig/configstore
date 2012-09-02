TARGET=

all:
	make -C sql_db $(TARGET) INC=$(shell pwd)/sql_db LIBDIR=$(shell pwd)/lib

install: 
	mkdir -p lib
	make TARGET=install

clean:
	make TARGET=clean
	rm -rf lib
