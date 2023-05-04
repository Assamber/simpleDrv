TARGET := Drv
PWD := $(shell pwd)
KERNEL := $(shell uname -r)

obj-m := $(TARGET).o
$(TARGET)-objs += drv_src.o

all:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KERNEL)/build M=$(PWD) clean
