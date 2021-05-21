all: attack

attack: attack.o
	cc -g -o $@ $^

%.o: %.c
	cc -g  -Wall -o $@ -c $<

clean:
	rm attack attack.o
