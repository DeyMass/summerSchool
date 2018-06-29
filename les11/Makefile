main: clear
	mkdir obj bin
	gcc -g -c ./src/main.c -o ./obj/main.o
	gcc -g ./obj/main.o -lncurses -o ./bin/res.out
	./bin/res.out

clear:
	rm -rf bin obj
