CFLAGS = -c -O2 -Wall -fPIC

all: tsclicker

tsclicker: tsclicker-obj
	gcc -o tsclicker.dll -shared plugin.o

tsclicker-obj: ./src/plugin.c
	gcc -Iinclude src/plugin.c $(CFLAGS)

clean:
	rm -rf *.o tsclicker.dll
