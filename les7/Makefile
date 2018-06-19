main: clear obj
	mkdir -p bin
	gcc ./obj/main.o -o ./bin/res.out

obj:
	mkdir -p obj
	gcc -c ./src/main.c -o ./obj/main.o

run: main
	./bin/res.out

clear:
	rm -rf bin
	rm -rf obj

