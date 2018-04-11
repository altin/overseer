#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Altin Rexhepaj and Randy Taylor");
MODULE_DESCRIPTION("overseer module");

static int __init overseer_init(void) {
    printk(KERN_INFO "overseer module loaded\n");
    return 0;
}

static void __exit overseer_cleanup(void) {
    printk(KERN_INFO "overseer module unloaded\n");
}

module_init(overseer_init);
module_exit(overseer_cleanup);
