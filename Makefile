all: attack

attack: server.c client.c defs.h
	gcc -o client client.c
	gcc -o server server.c

clean: 
	rm -f *.o
