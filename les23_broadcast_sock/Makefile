main: clear
	mkdir -p bin obj
	gcc -c ./src/broadcast.c -o ./obj/broadcast.o
	gcc ./obj/broadcast.o -o ./bin/broadcast.out
	gcc -c ./src/broadcastSrv.c -o ./obj/broadcastSrv.o
	gcc ./obj/broadcastSrv.o -o ./bin/broadcastSrv.out
	gcc -c ./src/multicast.c -o ./obj/multicast.o
	gcc ./obj/multicast.o -o ./bin/multicast.out
	gcc -c ./src/multicastSrv.c -o ./obj/multicastSrv.o
	gcc ./obj/multicastSrv.o -o ./bin/multicastSrv.out

clear:
	rm -rf bin obj
