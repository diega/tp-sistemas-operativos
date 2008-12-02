#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<linux/kd.h>

#define ERROR -1

void print_kbleds(long int arg);

int main(void) {
    int fd;
    long int state;

    if((fd = open("/dev/tty", O_NOCTTY)) == ERROR) {
        perror("open: Couldn't open /dev/tty");
        exit(ERROR);
    }

    if(ioctl(fd, KDGETLED, &state) == ERROR) {
        perror("ioctl: Couldn't do KDGETLED IOCTL");
        exit(ERROR);
    }

    print_kbleds(state);
    return 0;
}

void print_kbleds(long int state) {
    if (state & LED_SCR) printf("Scroll Lock LED is on.\n");
    if (state & LED_NUM) printf("Num Lock LED is on.\n");
    if (state & LED_CAP) printf("Caps Lock LED is on.\n");
}
