all: mutex multi_UDP_server multi_UDP_client

clean: 
	rm -f mutex mutex.o
	rm -f multi_UDP_server multi_UDP_server.o
	rm -f multi_UDP_client multi_UDP_client.o

mutex: mutex.o
	cc -g -o mutex mutex.o -pthread

multi_UDP_server: multi_UDP_server.o
	cc -g -o multi_UDP_server multi_UDP_server.o -pthread

multi_UDP_client: multi_UDP_client.o
	cc -g -o multi_UDP_client multi_UDP_client.o -pthread

%.o: %.c
	cc -g3 -Wall -o $@ -c $<

