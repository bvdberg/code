obj-m = test-wq.o
KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
	@rm -rf *.mod.* *.o Module.symvers .*.cmd .tmp_versions
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean

