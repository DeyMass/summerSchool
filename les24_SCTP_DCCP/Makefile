main: clear
	mkdir -p bin obj
	gcc -c ./src/SCTP_client.c -o ./obj/client.o
	gcc ./obj/client.o -lsctp -o ./bin/client.out
	gcc -c ./src/SCTP_server.c -o ./obj/server.o
	gcc ./obj/server.o -lsctp -o ./bin/server.out

clear:
	rm -rf bin obj
