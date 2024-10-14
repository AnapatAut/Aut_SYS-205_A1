#ifndef CHARDEV_H 
#define CHARDEV_H 
 
#include <linux/ioctl.h> 
#define HEARTYDEV_WRITE_CNT _IOR(10, 0, int) 
#define HEARTYDEV_READ_CNT _IOR(10, 1, int) 
#define HEARTYDEV_BUF_LEN _IOR(10, 2, int) 
#define HEARTYDEV_SET_MODE _IOWR(10, 3, int)

#define HEARTYDEV_MODE_NORMAL 1
#define HEARTYDEV_MODE_UPPER 2
#define HEARTYDEV_MODE_LOWER 3

#define BUF_LEN 100

#define DEVICE_PATH "/dev/ioctl" 
 
#endif