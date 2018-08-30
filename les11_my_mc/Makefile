main: clean
	mkdir obj bin
	gcc -g -c ./src/main.c -o ./obj/main.o
	gcc -g ./obj/main.o -lncurses -lpthread -o ./bin/res.out

run: main
	./bin/res.out

debug: main
	gdb ./bin/res.out

check: main
	valgrind --leak-check=full ./bin/res.out

clean:
	rm -rf bin obj *.o *.out *.rpm* *.deb
