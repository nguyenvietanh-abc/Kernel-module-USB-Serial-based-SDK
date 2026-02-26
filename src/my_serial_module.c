#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEVICE_NAME "my_serial"
#define CLASS_NAME  "my_serial"

static int major_number;
static struct class *my_class = NULL;
static struct cdev my_cdev;

static char message[256] = {0};
static int size_of_message = 0;

static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "my_serial: Device opened\n");
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "my_serial: Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    int error_count = copy_to_user(buffer, message, size_of_message);
    if (error_count == 0) {
        printk(KERN_INFO "my_serial: Sent %d characters to user\n", size_of_message);
        return size_of_message;
    } else {
        printk(KERN_INFO "my_serial: Failed to send data to user\n");
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    if (len > 255) len = 255;
    copy_from_user(message, buffer, len);
    size_of_message = len;
    printk(KERN_INFO "my_serial: Received %zu bytes from user\n", len);
    
    // Simulate decode radar signal (giống công việc anh làm)
    if (strstr(message, "RADAR_CMD") != NULL) {
        strcpy(message, "RADAR_RESP: Vehicle detected - Speed 45km/h - Lane 2");
        size_of_message = strlen(message);
        printk(KERN_INFO "my_serial: Simulated Radar decode OK\n");
    }
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init my_module_init(void) {
    printk(KERN_INFO "my_serial: Initializing kernel module for USB-Serial\n");
    
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "my_serial: Failed to register major number\n");
        return major_number;
    }

    my_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(my_class);
    }

    device_create(my_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    printk(KERN_INFO "my_serial: Device created successfully at /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit my_module_exit(void) {
    device_destroy(my_class, MKDEV(major_number, 0));
    class_destroy(my_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "my_serial: Module removed\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nguyen - Embedded Linux Engineer");
MODULE_DESCRIPTION("Custom USB-Serial character device driver for AI Traffic Camera BSP - supports Radar test via USB-to-UART");
MODULE_VERSION("1.0");