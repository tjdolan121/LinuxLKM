obj-m:= my_driver.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=/home/kernel/my_driver modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=/home/kernel/my_driver modules
