all: client server

client:
	gcc -c client.c; gcc -o client client.o

server:
	gcc -c server.c; gcc -o server server.o

clean:
	rm -f *.o client server

.PHONY: all clean client server