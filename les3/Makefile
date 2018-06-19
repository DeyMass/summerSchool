CC = gcc
main: clear
	mkdir bin
	mkdir obj
	$(CC) -c ./src/main.c -o ./obj/main.o
	$(CC) ./obj/main.o -o ./bin/res.out
clear:
	rm -rf bin
	rm -rf obj
start: main
	./bin/res.out
