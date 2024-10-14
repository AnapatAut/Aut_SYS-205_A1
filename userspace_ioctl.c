/* 
 * userspace_ioctl.c
 * 
 * Control the kernel module using ioctl
 */ 
 
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 
#include <sys/ioctl.h>

#include "chardev.h"  
 
// Return the number of times that heartydev_write is called
int heartydev_write_cnt(int file_desc) 
{ 
    int count; 
    count = ioctl(file_desc, HEARTYDEV_WRITE_CNT, NULL); 
 
    if (count < 0)
    { 
        printf("heartydev_write_cnt failed:%d\n", count); 
    }

    printf("Write count: %d\n", count);
    return count; 
} 
 
// Return the number of times that heartydev_read is called
int heartydev_read_cnt(int file_desc) 
{ 
    int count; 
    count = ioctl(file_desc, HEARTYDEV_READ_CNT, NULL); 
 
    if (count < 0)
    { 
        printf("heartydev_read_cnt failed:%d\n", count); 
    }

    printf("Read count: %d\n", count);
    return count; 
} 
 
// Return the length of the string in the buffer
int heartydev_buf_len(int file_desc) 
{ 
    int count; 
    count = ioctl(file_desc, HEARTYDEV_BUF_LEN, NULL); 
 
    if (count < 0)
    { 
        printf("heartydev_buf_len failed:%d\n", count); 
    }

    printf("Buffer length: %d\n", count);
    return count;  
} 

// Set the mode for heartydev_read
// "int mode" is the key to set the mode
int heartydev_set_mode(int file_desc, int mode) 
{ 
    int retval; 
    retval = ioctl(file_desc, HEARTYDEV_SET_MODE, mode); 
 
    if(retval < HEARTYDEV_MODE_NORMAL && retval > HEARTYDEV_MODE_LOWER)
    { 
        printf("heartydev_set_mode failed:%d\n", retval); 
    }

    switch(mode)
    {
    case HEARTYDEV_MODE_NORMAL:
        printf("Mode: NORMAL\n");
        break;
    case HEARTYDEV_MODE_UPPER:
        printf("Mode: UPPER\n");
        break;
    case HEARTYDEV_MODE_LOWER:
        printf("Mode: LOWER\n");
        break;
    default:
        break;
    }
 
    return retval; 
} 

// Main function to call the ioctl functions
int main(void) 
{ 
    int file_desc; 
    char *msg = "Message passed by ioctl\n"; 
    int mode = HEARTYDEV_MODE_NORMAL;

    file_desc = open(DEVICE_PATH, O_RDWR); 
    if (file_desc < 0)
    { 
        printf("Can't open device file: %s, error:%d\n", DEVICE_PATH, file_desc); 
        exit(EXIT_FAILURE); 
    } 

    printf("HEARTYDEV_WRITE_CNT\n");
    heartydev_write_cnt(file_desc);

    printf("\nHEARTYDEV_READ_CNT\n");
    heartydev_read_cnt(file_desc); 

    printf("\nHEARTYDEV_BUF_LEN\n");
    heartydev_buf_len(file_desc); 

    printf("\nHEARTYDEV_SET_MODE\n");
    heartydev_set_mode(file_desc, mode); 

    close(file_desc); 
    return 0; 
error: 
    close(file_desc); 
    exit(EXIT_FAILURE); 
}