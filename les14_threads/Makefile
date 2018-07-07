main:
	mkdir -p obj bin
	gcc -c ./src/threadId.c -o ./obj/threadId.o
	gcc ./obj/threadId.o -lpthread -o ./bin/res.out
	gcc -c ./src/lifeSimulator.c -o ./obj/life.o
	gcc ./obj/life.o -lpthread -o ./bin/life.out
	gcc -g -c ./src/my_errno.c -o ./obj/errno.o
	gcc -g ./obj/errno.o -lpthread -o ./bin/errno.out

clear:
	rm -rf bin obj

life: main
	./bin/life.out

errno: main
	./bin/errno.out

threadid: main
	./bin/res.out
