/*
 * main.c
 *
 * Functions for heartydev driver
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#include "chardev.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple character device driver");
MODULE_AUTHOR("... <...@cmkl.ac.th>");

static int heartydev_open(struct inode *inode, struct file *file);
static int heartydev_release(struct inode *inode, struct file *file);
static long heartydev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t heartydev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t heartydev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

static const struct file_operations heartydev_fops =
{
    .open = heartydev_open,
    .release = heartydev_release,
    .unlocked_ioctl = heartydev_ioctl,
    .read = heartydev_read,
    .write = heartydev_write
};

dev_t dev = 0;
static struct class *heartydev_class = NULL;
static struct cdev heartydev_cdev;

uint8_t data[BUF_LEN + 1];
uint8_t read_mode = HEARTYDEV_MODE_UPPER;
int write_count = 0, read_count = 0;

// Heartydev event
static int heartydev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

// Initialize heartydev
static int __init heartydev_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "heartydev") < 0)
    {
        printk(KERN_ALERT "heartydev registration failed\n");
        return -1;
    }

    heartydev_class = class_create(THIS_MODULE, "heartydev");
    heartydev_class->dev_uevent = heartydev_uevent;
    cdev_init(&heartydev_cdev, &heartydev_fops);
    cdev_add(&heartydev_cdev, MKDEV(MAJOR(dev), 0), 1);
    device_create(heartydev_class, NULL, MKDEV(MAJOR(dev), 0), NULL, "heartydev");
    printk(KERN_ALERT "heartydev registered\n");
    return 0;
}

// Exit heartydev
static void __exit heartydev_exit(void)
{
    device_destroy(heartydev_class, MKDEV(MAJOR(dev), 0));
    class_unregister(heartydev_class);
    class_destroy(heartydev_class);
    unregister_chrdev_region(MKDEV(MAJOR(dev), 0), MINORMASK);
}

// Open heartydev
static int heartydev_open(struct inode *inode, struct file *file)
{
    printk("heartydev_open\n");
    return 0;
}

// Release heartydev
static int heartydev_release(struct inode *inode, struct file *file)
{
    printk("heartydev_release\n");
    return 0;
}

// Ioctl function for heartydev
// Returns read count, write count, buffer length, or set mode
// "cmd" is the command chosed
// "arg" is the mode chosen for set mode function
static long heartydev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("heartydev_ioctl\n");
    int val;

    switch(cmd)
    {
    case HEARTYDEV_WRITE_CNT:
        val = write_count;
        break;
    case HEARTYDEV_READ_CNT:
        val = read_count;
        break;
    case HEARTYDEV_BUF_LEN:
        val = strlen(data) - 1;
        break;
    case HEARTYDEV_SET_MODE:
        if(arg >= HEARTYDEV_MODE_NORMAL && arg <= HEARTYDEV_MODE_LOWER)
        {
            val = read_mode;
            read_mode = arg;
        }
        break;
    default:
        return 0;
    }
    return val;
}

// Read data from memory and returns data
static ssize_t heartydev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    printk("heartydev_read\n");
    size_t datalen = strlen(data);

    if(*offset >= datalen - 1)
    {
        return 0;
    }

    if(count > datalen - *offset)
    {
        count = datalen - *offset;
    }

    int i = 0;
    char data_mode[count];
    while(i < count)
    {
        char c = data[i];
        if(read_mode == HEARTYDEV_MODE_UPPER && (c >= 97 && c <= 122))
        {
            c -= 32;
        }

        if(read_mode == HEARTYDEV_MODE_LOWER && (c >= 65 && c <= 90))
        {
            c += 32;
        }

        data_mode[i] = c;
        i++;
    }

    if(copy_to_user(buf, data_mode, count))
    {
        return -EFAULT;
    }
    *offset += count;
    read_count++;
    return count;
}

// Write data from user buffer to memory
static ssize_t heartydev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    printk("heartydev_write\n");
    size_t maxdatalen = BUF_LEN, ncopied;

    if(*offset >= count - 1)
    {
        return -1;
    }

    if(maxdatalen > count)
    {
        maxdatalen = count;
    }
    ncopied = copy_from_user(data, buf, maxdatalen);
    maxdatalen = strlen(data);
    if (ncopied == 0)
    {
        printk("Copied %zd bytes from the user\n", maxdatalen);
    }
    else
    {
        printk("Could't copy %zd bytes from the user\n", ncopied);
    }
    data[maxdatalen] = '\0';

    printk("Data from the user: %s", data);

    write_count++;
    return maxdatalen;
}

module_init(heartydev_init);
module_exit(heartydev_exit);