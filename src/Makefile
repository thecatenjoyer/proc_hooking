obj-m := proc_hook.o
proc_hook-objs = entry.o hook.o
CC = gcc -Wall -std=c99
KERNEL_HEADERS := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_HEADERS) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_HEADERS) M=$(PWD) clean
