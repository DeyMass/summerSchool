main: clean
	mkdir -p obj
	gcc -c ./src/reverse.c -o ./obj/reverse.o
	gcc -c ./src/main.c -o ./obj/main.o
	mkdir -p bin
	gcc ./obj/reverse.o -o ./bin/reverse.out
	gcc ./obj/main.o -lncurses -o ./bin/res.out
	./bin/res.out

clean:
	rm -rf obj bin


