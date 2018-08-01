main: clear
	mkdir -p bin obj
	gcc -std=c99 -g -c ./src/main.c -o ./obj/main.o
	gcc -std=c99 -g ./obj/main.o -lpthread -o ./bin/res.out
	sudo ./bin/res.out

clear:
	rm -rf bin obj
