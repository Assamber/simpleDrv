#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("Dual MIT/GPL");

int __init test_drv_init(void)
{
    printk("test_drv_init called\n");
    return 0;
}
void __exit test_drv_exit(void)
{
    printk("test_drv_exit called\n");
}

module_init(test_drv_init);
module_exit(test_drv_exit);