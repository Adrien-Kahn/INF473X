REBUILDABLES = wgetX.o test_url.o url.o test_url wgetX

all : wgetX test_url


clean: 
	rm -f $(REBUILDABLES)

wgetX : wgetX.o url.o
	cc -g -o $@ $^ 

test_url : test_url.o url.o
	cc -g -o $@ $^ 

%.o : %.c
	cc -g  -Wall -o $@ -c $<

wgetX.o : wgetX.h url.h
url.o : url.h
test_url.o: url.h url.c
 
