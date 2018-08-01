main:
	mkdir -p bin obj
	gcc -c ./src/ping.c -o ./obj/ping.o
	gcc ./obj/ping.o -o ./bin/ping.out
	gcc -c ./src/tracert.c -o ./obj/trace.o
	gcc ./obj/trace.o -o ./bin/trace.out

clear:
	rm -rf bin obj
