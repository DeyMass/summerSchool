main: clear
	mkdir -p bin obj
	gcc -c ./src/raw_client.c -o ./obj/raw_client.o
	gcc ./obj/raw_client.o -o ./bin/raw_client.out

clear:
	rm -rf bin obj
