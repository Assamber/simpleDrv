#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>


#include "../../Drv/drv_ioctl.h"

#define DEVICE_NAME "/dev/DRiVeerDNA"

void err_show(void)
{
    printf("Error = %s\n", strerrorname_np(errno));
}

int main(void)
{
    int device = open(DEVICE_NAME"0", O_RDWR);

    if(device < 0)
    {
        err_show();
        return -ENODATA;
    }

    simpleDrv_ioctl_data_t data = {0};
    printf("---IOCTL---\n");

    data.inputData = "Ioctl for driver";
    data.inputLength = strlen(data.inputData) + 1;
    ioctl(device, IOCTL_BLOCK_DRV_SET, &data);
}