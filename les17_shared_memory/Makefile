main:
	mkdir -p bin obj
	gcc -g -c ./src/client.c -o ./obj/client.o
	gcc -g ./obj/client.o -lpthread -o ./bin/client.out
	gcc -g -c ./src/server.c -o ./obj/server.o
	gcc -g ./obj/server.o -lpthread -o ./bin/server.out

clear:
	rm -rf bin obj
