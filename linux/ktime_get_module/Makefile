obj-m = module.o
KPATH = ../../thales_linux-omapl1

all:
	make CROSS_COMPILE=arm-none-linux-gnueabi- ARCH=arm -C $(KPATH) M=$(PWD) modules
	@rm -rf *.mod.* *.o Module.symvers .*.cmd .tmp_versions modules.order

clean:
	make -C $(KPATH) M=$(PWD) clean

