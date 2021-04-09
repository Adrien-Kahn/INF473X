all: udp_client udp_server fancy_client

udp_client: udp_client.o
	cc -g -o $@ $^

fancy_client: fancy_client.o
	cc -g -o $@ $^

udp_server: udp_server.o
	cc -g -o $@ $^

%.o: %.c
	cc -g  -Wall -o $@ -c $<

clean:
	rm udp_client udp_client.o
	rm udp_server udp_server.o
	rm fancy_client fancy_client.o
