obj-m += keyboard_leds_module.o

all: print_kbleds
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

print_kbleds:check_kbleds.c
	gcc $? -o $@

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
