main: clear
	mkdir -p obj bin
	gcc -c ./src/main.c -o ./obj/main.o
	gcc ./obj/main.o -o ./bin/res.out
	gcc -c ./src/interpretator.c -o ./obj/inter.o
	gcc ./obj/inter.o -o ./bin/interpretator.out
	#ls -l | ./bin/res.out

clear:
	rm -rf bin obj
