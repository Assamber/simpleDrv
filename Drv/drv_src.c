#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/blkdev.h>

#include "drv_src.h"

MODULE_LICENSE("Dual MIT/GPL");

static int deviceMajorNum = 0;
static struct dendisk* deviceGenDisk = NULL;
static struct request_queue* deviceQueue = NULL;

static int  simpleDrv_api_open(struct block_device* device, fmode_t mode);
static void simpleDrv_api_release(struct gendisk* genDisk, fmode_t mode);
static int simpleDrv_api_ioctl(struct block_device* device, fmode_t mode, unsigned int cmd, unsigned long arg);

static struct block_device_operations simpleDrv_block_fuctions = {
    .owner = THIS_MODULE,
    .open = simpleDrv_api_open,
    .release = simpleDrv_api_release,
    .ioctl = simpleDrv_api_ioctl
};

int __init simpleDrv_init(void)
{
    DBGMSG("simpleDrv_init called\n");
    deviceMajorNum = register_blkdev(DEVICE_MAJOR, DEVICE_NAME);

    if(deviceMajorNum < 0)
    {
        DBGERR("Unable to register device!\n");
        return -EBUSY;
    }
    
    return 0;
}
void __exit simpleDrv_exit(void)
{
    unregister_blkdev(deviceMajorNum, DEVICE_NAME);
    DBGMSG("simpleDrv_exit called\n");
}

module_init(simpleDrv_init);
module_exit(simpleDrv_exit);