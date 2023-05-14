#ifndef __DRV_IOCTL_H__
#define __DRV_IOCTL_H__

#include <linux/ioctl.h>

#define BLOCK_DRV_MAGIC 0xAF

#define BLOCK_DRV_GET         0x70
#define BLOCK_DRV_SET         0x71
#define BLOCK_DRV_GET_AND_SET 0x72
#define BLOCK_DRV_DBG_MESSAGE 0x73

typedef struct simpleDrv_ioctl_data_s
{
    char*  inputData;
    size_t inputLength;
    char*  outputData;
    size_t outputLength;
    size_t* returnedSize;
}simpleDrv_ioctl_data_t;

#define IOCTL_BLOCK_DRV_GET _IOR(BLOCK_DRV_MAGIC, BLOCK_DRV_GET, char*)
#define IOCTL_BLOCK_DRV_SET _IOW(BLOCK_DRV_MAGIC, BLOCK_DRV_SET, char*)
#define IOCTL_BLOCK_DRV_GET_AND_SET _IOWR(BLOCK_DRV_MAGIC, BLOCK_DRV_GET_AND_SET, char*)
#define IOCTL_BLOCK_DRV_DBG_MESSAGE _IO(BLOCK_DRV_MAGIC, BLOCK_DRV_DBG_MESSAGE)

#endif