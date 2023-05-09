#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/blkdev.h>

#include "drv_src.h"
#include "drv_ioctl.h"

MODULE_LICENSE("Dual MIT/GPL");

static int deviceMajorNum = 0;
static struct gendisk* deviceGenDisk = NULL;
static struct request_queue* deviceQueue = NULL;
static int deviceDataLength = 0;
static unsigned char* mainBuffer = 0;

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
    int err = 0;

    DBGMSG("simpleDrv_init called\n");
    deviceMajorNum = register_blkdev(DEVICE_MAJOR, DEVICE_NAME);
    if(deviceMajorNum < 0)
    {
        DBGERR("Unable to register device!\n");
        return -EBUSY;
    }
    else
        DBGMSG("Device Major = %d\n", deviceMajorNum);

    deviceGenDisk = blk_alloc_disk(DEVICE_DISK_MINOR);
    if(deviceGenDisk == NULL)
    {
        DBGERR("Unable to allocate disk!\n");
        unregister_blkdev(deviceMajorNum, DEVICE_NAME);
        return -ENOMEM;
    }

    mainBuffer = kzalloc(DEVICE_BUFFER_SIZE, GFP_KERNEL);
    if(mainBuffer == NULL)
    {       
        DBGERR("Unable to allocate memory buffer!\n");
        unregister_blkdev(deviceMajorNum, DEVICE_NAME);
        return -ENOMEM;
    }
    deviceDataLength = 0;

    deviceGenDisk->major = deviceMajorNum;
    deviceGenDisk->first_minor = 0;
    deviceGenDisk->minors = DEVICE_DISK_MINOR;
    deviceGenDisk->fops = &simpleDrv_block_fuctions;
    //deviceGenDisk->queue = blk_init_queque
    deviceGenDisk->flags = GENHD_FL_NO_PART;
    
    strcpy(deviceGenDisk->disk_name, DEVICE_NAME"0");
    set_capacity(deviceGenDisk, NR_SECTORS);

    //Last step activate drv func
    err = add_disk(deviceGenDisk); 
    if(err)
    {
        DBGERR("Unable to add disk!\n");
        kfree(mainBuffer);
        put_disk(deviceGenDisk);
        unregister_blkdev(deviceMajorNum, DEVICE_NAME);
        return -ENOMEM;
    }

    DBGMSG("simpleDrv_init completed\n");
    return 0;
}
void __exit simpleDrv_exit(void)
{
    DBGMSG("simpleDrv_exit called\n");

    kfree(mainBuffer);

    del_gendisk(deviceGenDisk);
    put_disk(deviceGenDisk);

    unregister_blkdev(deviceMajorNum, DEVICE_NAME);
    //blk_cleanup_queue(deviceMajorNum->qieue);   
}

static int  simpleDrv_api_open(struct block_device* device, fmode_t mode)
{
    DBGMSG("simpleDrv_open called\n");
    return 0;
}

static void simpleDrv_api_release(struct gendisk* genDisk, fmode_t mode)
{
    DBGMSG("simpleDrv_release called\n");
}

static int simpleDrv_api_ioctl(struct block_device* device, fmode_t mode, unsigned int cmd, unsigned long arg)
{
    simpleDrv_ioctl_data_t* data = NULL;

    DBGMSG("simpleDrv_ioctl called\n");

    data = (simpleDrv_ioctl_data_t*)arg;

    switch (cmd)
    {
        case IOCTL_BLOCK_DRV_GET:
             DBGMSG("IOCTL_GET called\n"); break;
        case IOCTL_BLOCK_DRV_SET:
             DBGMSG("IOCTL_SET called\n"); break;
        case IOCTL_BLOCK_DRV_GET_AND_SET:
             DBGMSG("IOCTL_GET_AND_SET called\n"); break;
        case IOCLT_BLOCK_DRV_DBG_MESSAGE:
             DBGMSG("IOCTL_DBG called\n"); break;
    
        default: break;
    }

    return 0;
}

module_init(simpleDrv_init);
module_exit(simpleDrv_exit);