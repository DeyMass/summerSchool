main: operations
	gcc -g ./obj/* -lm -o ./bin/res.out

operations: createDirs
	gcc -g -c ./src/main.c -o ./obj/main.o
	gcc -g -c ./src/add.c -o ./obj/add.o
	gcc -g -c ./src/div.c -o ./obj/div.o
	gcc -g -c ./src/sub.c -o ./obj/sub.o
	gcc -g -c ./src/mul.c -o ./obj/mul.o
	gcc -g -c ./src/div.c -o ./obj/div.o
	gcc -g -c ./src/log.c -o ./obj/log.o

createDirs:
	mkdir -p obj
	mkdir -p bin

clear:
	rm -rf ./bin
	rm -rf ./obj
	rm -f *.o

make: obj/main.o obj/add.o obj/sub.o obj/mul.o obj/div.o obj/log.o
	gcc -g obj/main.o obj/add.o obj/sub.o obj/mul.o obj/div.o obj/log.o -lm
