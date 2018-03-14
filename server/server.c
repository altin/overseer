#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "overseer"
#define CLASS_NAME  "overseer_class"

MODULE_DESCRIPTION("overseer - server");
MODULE_LICENSE("GPL");

// automatically determined device number 
static int major_number;   
// data from userspace               
static char message[256] = {0};
// its size         
static short size_of_message;  
// number of times the device is opened           
static int number_opens = 0;
// driver class struct pointer          
static struct class* driver_class  = NULL;
// driver device struct pointer
static struct device* driver_device = NULL;

// driver prototype functions 
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/* Driver - operation associations */
static struct file_operations fops = {
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};

/* Initialization function */
static int __init overseer_init(void) {
    printk(KERN_INFO "overseer: initializing\n");
    // dynamically allocate a major #
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "overseer: failed to allocate major number\n");
        return major_number;
    }
    printk(KERN_INFO "overseer: registered with major number %d\n", major_number);

    // register device class
    driver_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(driver_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "overseer: failed to register device class\n");
        return PTR_ERR(driver_class);
    }
    printk(KERN_INFO "overseer: device class registered\n");

    // register device driver
    driver_device = device_create(driver_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(driver_device)) {
        // if error, clean up
        class_destroy(driver_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "overseer: failed to create device\n");
        return PTR_ERR(driver_device);
    }
    printk(KERN_INFO "overseer: device class created\n");

    return 0;
}

/* Cleanup */
static void __exit overseer_exit(void) {
    // remove device
    device_destroy(driver_class, MKDEV(major_number, 0));
    // unregister device class
    class_unregister(driver_class);
    // remove device class
    class_destroy(driver_class);
    // unregister major number
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "overseer: closed\n");
}

/* Called each time the device is opened.
 * inodep = ptr to inode
 * file = ptr to file object
 */
static int dev_open(struct inode *inodep, struct file *filep) {
    // increment numberOpens counter
    number_opens++;
    printk(KERN_INFO "overseer: device opened %d time(s)\n", number_opens);
    return 0;
}

/* Called when device is read.
 * filep = pointer to a file
 * buffer = pointer to the buffer to which this function writes the data
 * len = length of buffer
 * offset = offset in buffer
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;

    // copy_to_user has format ( * to, *from, size), returns 0 on success
    error_count = copy_to_user(buffer, message, size_of_message);
    if (error_count == 0) { // success
        printk(KERN_INFO "overseer: sent %d characters to user\n", size_of_message);
        return (size_of_message = 0);
    } else {
        printk(KERN_INFO "overseer: failed to send %d characters to the user\n", error_count);
        return -EFAULT;
    }
}

/* Called whenever device is written.
 * filep = pointer to file
 * buffer = pointer to buffer that contains data to write to the device
 * len = length of data
 * offset = offset in buffer
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    // append received string and its length
    sprintf(message, "%s .(%d letters)", buffer, (int)len);
    // stor length of message
    size_of_message = strlen(message);
    printk(KERN_INFO "overseer: received %d characters from the user\n", (int)len);
    return len;
}

/* Called when device is closed/released.
 * inodep = pointer to inode
 * filep = pointer to a file
 */
static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "overseer: released\n");
    return 0;
}

module_init(overseer_init);
module_exit(overseer_exit);
