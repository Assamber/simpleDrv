#ifndef __DRV_SRC_H__
#define __DRV_SRC_H__

#define DEVICE_NAME "SIMPLE_DRV"
#define DBGMSG(fmt, ...) printk(DEVICE_NAME": "fmt,##__VA_ARGS__)

#endif //__DRV_SRC_H__