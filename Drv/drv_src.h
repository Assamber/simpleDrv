#ifndef __DRV_SRC_H__
#define __DRV_SRC_H__

#define DEVICE_NAME "DRiVeerDNA"
#define DEVICE_MAJOR 0 //Free major in sys
#define DEVICE_DISK_MINOR 1 //Alloc disk with minor 1
#define DEVICE_BUFFER_SIZE 1024

#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt,##__VA_ARGS__)
#define DBGERR(fmt, ...) printk(KERN_ERR DEVICE_NAME": "fmt,##__VA_ARGS__)

#endif //__DRV_SRC_H__