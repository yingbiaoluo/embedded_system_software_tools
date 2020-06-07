ifneq ($(KERNELRELEASE),)
obj-m:=scull_rpi.o
else
KERNELDIR:=/home/biao/linux
PWD:=$(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
clean:
	rm -f *.ko *.o *.mod.c *.symvers modul*
endif
