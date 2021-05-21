all: attack selfAttack app rev_shellcode

attack: attack.o
	cc -g -o $@ $^
	
selfAttack: selfAttack.o
	cc -g -o $@ $^

app: app.o
	cc -g -o $@ $^

rev_shellcode: rev_shellcode.o
	cc -g -o $@ $^

%.o: %.c
	cc -g -z execstack -fno-stack-protector -Wall -o $@ -c $<

clean:
	rm attack attack.o
	rm app app.o
	rm selfAttack selfAttack.o
	rm rev_shellcode rev_shellcode.o
