all: OverflowAttack app

OverflowAttack: OverflowAttack.o
	cc -g -o $@ $^

app: app.o
	cc -g -o $@ $^

%.o: %.c
	cc -g  -Wall -o $@ -c $<

clean:
	rm OverflowAttack OverflowAttack.o
	rm app app.o
