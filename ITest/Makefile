all: itest

itest: itest.o
	cc -g -o itest itest.o

hello-world.o: hello-world.c
	cc -c -Wall -g itest.c

clean:
	rm itest.o itest
