all: mutex multi_UDP_server

clean: 
	rm -f mutex mutex.o
	rm -f multi_UDP_server multi_UDP_server.o

mutex: mutex.o
	cc -g -o mutex mutex.o -pthread

multi_UDP_server: multi_UDP_server.o
	cc -g -o multi_UDP_server multi_UDP_server.o -pthread

%.o: %.c
	cc -g3 -Wall -o $@ -c $<

