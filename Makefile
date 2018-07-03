main:
	make -C ./les1\(*/
	make -C ./les2*/
	make -C ./les13*/

test:
	gcc ./les4*/testing.c -lm -o test.out
	./test.out
