C:=gcc
CFLAGS:=-I. -fpic
SRC := src/sockutil.c
OBJ := build/sockutil.o

SRC2 := src/filetransfer.c
OBJ2 := build/filetransfer.o
all: build/libsockutil.so build/sockutil.a build/libfiletransfer.so build/filetransfer.a

build/libsockutil.so: $(OBJ)
	$(CC) -shared -o $@ $^

build/sockutil.a: $(OBJ)
	ar -rs $@ $^

build/sockutil.o: 
	mkdir -p build/include
	cp src/sockutil.h build/include/sockutil.h
	$(CC) -c -o $@ $(SRC) $(CFLAGS)

build/libfiletransfer.so: $(OBJ2)
	$(CC) -shared -o $@ $^

build/filetransfer.a: $(OBJ2)
	ar -rs $@ $^

build/filetransfer.o: 
	mkdir -p build/include
	cp src/filetransfer.h build/include/filetransfer.h
	$(CC) -c -o $@ $(SRC2) $(CFLAGS)

test:
	mkdir -p build/test
	mkdir -p bin/test 
	$(CC) -c test/server.c -o build/test/server.o
	$(CC) -c test/client.c -o build/test/client.o
	$(CC) -o bin/test/server build/test/server.o build/sockutil.a build/filetransfer.a
	$(CC) -o bin/test/client build/test/client.o build/sockutil.a build/filetransfer.a

clean:
	rm -rf build
	rm -rf bin

.PHONY: all test clean
