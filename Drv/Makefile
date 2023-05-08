# https://www.kernel.org/doc/Documentation/kbuild/modules.txt
TARGET := Drv
PWD := $(shell pwd)
KERNEL := $(shell uname -r)

obj-m := $(TARGET).o
$(TARGET)-objs += drv_src.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean

install:
	sudo insmod Drv.ko

uninstall:
	sudo rmmod Drv
