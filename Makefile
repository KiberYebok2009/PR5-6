TARGET_MODULE := simple-module

ifneq ($(KERNELRELEASE),)
    obj-m := $(TARGET_MODULE).o
    $(TARGET_MODULE)-objs := main.o device_file.o
else
    KDIR := /lib/modules/$(shell uname -r)/build
    PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -a
endif
