obj-m += ping.o

all:
	make -C /lib/modules/4.15.0-33-generic/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/4.15.0-33-generic/build M=$(shell pwd) clean
