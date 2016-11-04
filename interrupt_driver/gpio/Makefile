

export CC=${CROSS_COMPILE}gcc
export ARCH=arm

# if KERNELRELEASE is defined we've been invoked from the
# Kernel build system and can use it's language.
ifneq ($(KERNELRELEASE),)
	obj-m := gpio_interrupt.o 
	gpio_interrupt := gpio_interrupt.o
	# otherwise we were called directly and need to invoke kernel build system
else
	KERNELDIR ?= "/home/bclouser/workspace/kernel/release/3.10-9914410"
	PWD := $(shell pwd)

default: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif


clean:
	rm *.ko *.o *.mod.* *.symvers *.order
