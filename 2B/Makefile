all: udp_client

udp_client: udp_client.o
	cc -g -o $@ $^

%.o: %.c
	cc -g  -Wall -o $@ -c $<

clean:
	rm udp_client udp_client.o
