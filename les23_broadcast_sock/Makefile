main: clear
	mkdir -p bin obj
	gcc -c ./src/broadcast.c -o ./obj/broadcast.o
	gcc ./obj/broadcast.o -o ./bin/broadcast.out
	gcc -c ./src/broadcastSrv.c -o ./obj/broadcastSrv.o
	gcc ./obj/broadcastSrv.o -o ./bin/broadcastSrv.out

clear:
	rm -rf bin obj
