main: clear
	mkdir obj bin
	gcc -g -c ./src/main.c -o ./obj/main.o
	gcc -g ./obj/main.o -lncurses -o ./bin/res.out

run: main
	./bin/res.out

debug: main
	gdb ./bin/res.out

check: main
	valgrind --leak-check=full ./bin/res.out

clear:
	rm -rf bin obj
