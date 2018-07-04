main: obj
	mkdir -p bin
	gcc -g ./obj/* -o ./bin/res.out

obj:
	mkdir -p obj
	gcc -g -c ./src/leak.c -o ./obj/leak.o

clear:
	rm -rf ./obj
	rm -rf ./bin
	rm -rf *.o
