#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>

#define DEVICE_NAME "Simple-driver"

static int major = 0;
static struct class *dev_class;
static struct device *dev_struct;

extern ssize_t device_file_read(struct file *, char __user *, size_t, loff_t *);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read  = device_file_read,
};

static int __init my_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
        return major;

    dev_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(dev_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(dev_class);
    }

    dev_struct = device_create(dev_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(dev_struct)) {
        class_destroy(dev_class);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(dev_struct);
    }

    printk(KERN_NOTICE "Simple-driver: loaded, major = %d\n", major);
    return 0;
}

static void __exit my_exit(void)
{
    device_destroy(dev_class, MKDEV(major, 0));
    class_destroy(dev_class);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_NOTICE "Simple-driver: unloaded\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple char driver for lab 5-6");
