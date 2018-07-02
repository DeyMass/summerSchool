main:
	mkdir -p bin obj
	gcc -c ./src/main.c -o ./obj/main.o
	gcc ./obj/main.o -o ./bin/res.out

clear:
	rm -rf bin obj

run: main
	./bin/res.out
