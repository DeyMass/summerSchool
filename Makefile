main:
	make -C ./les1
	make -C ./les2
	make -C ./les3
	make -C ./les4
	make -C ./les7
	make -C ./les8
test:
	gcc ./les4/testing.c -lm -o test.out
	./test.out
