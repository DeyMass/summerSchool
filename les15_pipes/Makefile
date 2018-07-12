main: clear
	mkdir -p obj bin
	gcc -g -c ./src/main.c -o ./obj/main.o
	gcc -g ./obj/main.o -o ./bin/res.out
	gcc -g -c ./src/interpretator.c -o ./obj/inter.o
	gcc -g ./obj/inter.o -o ./bin/interpretator.out
	#ls -l | ./bin/res.out

clear:
	rm -rf bin obj
