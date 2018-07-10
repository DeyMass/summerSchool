main:
	mkdir -p bin obj
	gcc -g -c ./src/client.c -o ./obj/client.o
	gcc -g ./obj/client.o -lpthread -o ./bin/client
	gcc -g -c ./src/server.c -o ./obj/server.o
	gcc -g ./obj/server.o -lpthread -o ./bin/server

clear:
	rm -rf bin obj
