CFLAGS = -c -O2 -Wall -fPIC

all: tsclicker

tsclicker: tsclicker-obj
	gcc -o build/tsclicker.dll -shared build/plugin.o build/clicker.o

tsclicker-obj: clean
	mkdir -p build
	gcc -o build/plugin.o -Iinclude src/plugin.c $(CFLAGS)
	gcc -o build/clicker.o -Iinclude src/clicker.c $(CFLAGS)

clean:
	rm -rf build/*
