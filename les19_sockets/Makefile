main: clear
	mkdir -p bin obj
	gcc -g -c ./src/server.c -o ./obj/server.o
	gcc -g ./obj/server.o -o ./bin/server.out
	gcc -g -c ./src/client.c -o ./obj/client.o
	gcc -g ./obj/client.o -o ./bin/client.out

clear:
	rm -rf bin obj
