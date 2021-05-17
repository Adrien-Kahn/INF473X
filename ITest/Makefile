all: itest shellcode

itest: itest.o
	cc -g -fno-stack-protector -z execstack -o itest itest.o

shellcode: shellcode.o
	cc -g -fno-stack-protector -z execstack -o shellcode shellcode.o


hello-world.o: hello-world.c
	cc -c -Wall -g itest.c

%.o: %.c
	cc -g -z execstack -fno-stack-protector -Wall -o $@ -c $<

clean:
	rm itest.o itest
