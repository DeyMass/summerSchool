main:
	mkdir -p bin
	gcc -g -I./include ./src/main.c -ldl -o ./bin/res.out

createLib:
	mkdir -p obj
	mkdir -p lib
	gcc -fPIC -c ./src/log_Func.c -lm -o ./obj/log_Func.o
	gcc -fPIC -c ./src/add_Func.c -lm -o ./obj/add_Func.o
	gcc -fPIC -c ./src/sub_Func.c -lm -o ./obj/sub_Func.o
	gcc -fPIC -c ./src/div_Func.c -lm -o ./obj/div_Func.o
	gcc -fPIC -c ./src/mul_Func.c -lm -o ./obj/mul_Func.o
	gcc -shared ./obj/add_Func.o -o ./lib/add_Func.so
	gcc -shared ./obj/sub_Func.o -o ./lib/sub_Func.so
	gcc -shared ./obj/mul_Func.o -o ./lib/mul_Func.so
	gcc -shared ./obj/div_Func.o -o ./lib/div_Func.so
	gcc -shared ./obj/log_Func.o -o ./lib/log_Func.so
