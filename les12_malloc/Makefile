main:
	mkdir -p bin obj
	gcc -g -c ./src/main.c -o ./obj/main.o
	gcc -g ./obj/main.o -o ./bin/res.out

clear:
	rm -rf bin obj

run: main
	./bin/res.out
