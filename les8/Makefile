main: createDirs operations make_lib
	gcc ./obj/main.o -L./libs/ -lmathfunc -lm -o ./bin/res.out

operations:
	gcc -g -I./includes/ -c ./src/main.c -o ./obj/main.o
	gcc -g -c ./src/add.c -o ./obj/add.o
	gcc -g -c ./src/div.c -o ./obj/div.o
	gcc -g -c ./src/sub.c -o ./obj/sub.o
	gcc -g -c ./src/mul.c -o ./obj/mul.o
	gcc -g -c ./src/div.c -o ./obj/div.o
	gcc -g -c ./src/log.c -o ./obj/log.o

createDirs:
	mkdir -p obj
	mkdir -p bin
	mkdir -p libs

clear:
	rm -rf ./libs
	rm -rf ./bin
	rm -rf ./obj
	rm -f *.o

make_lib:
	ar -rc libs/libmathfunc.a ./obj/*.o
	ar -d libs/libmathfunc.a main.c

run: main
	./bin/res.out
