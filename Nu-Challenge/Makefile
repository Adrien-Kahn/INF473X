all: attack selfAttack app

attack: attack.o
	cc -g -o $@ $^
	
selfAttack: selfAttack.o
	cc -g -o $@ $^

app: app.o
	cc -g -o $@ $^

%.o: %.c
	cc -g -z execstack -fno-stack-protector -Wall -o $@ -c $<

clean:
	rm attack attack.o
	rm app app.o
	rm selfAttack selfAttack.o
