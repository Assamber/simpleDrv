#ifndef __DRV_SRC_H__
#define __DRV_SRC_H__

#define DEVICE_NAME "DRVeerdnA"
#define DEVICE_MAJOR 0 //Free major in sys
#define DEVICE_BUFFER_SIZE 1024

#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt,##__VA_ARGS__)
#define DBGERR(fmt, ...) printk(KERN_ERR DEVICE_NAME": "fmt,##__VA_ARGS__)

#endif //__DRV_SRC_H__