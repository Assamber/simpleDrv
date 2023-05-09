#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>


#include "../../Drv/drv_ioctl.h"

#define DEVICE_NAME "/dev/DRiVeerDNA"
#define BUFFER_LEN 1024

void err_show(void)
{
    printf("Error = %s\n", strerrorname_np(errno));
}

int main(void)
{
    char tempBuffer[BUFFER_LEN] = {0};

    int device = open(DEVICE_NAME"0", O_RDWR);
    if(device < 0)
    {
        err_show();
        return -ENODATA;
    }

    simpleDrv_ioctl_data_t data = {0};
    printf("---IOCTL---\n");

    data.inputData = "Set this message to driver buffer!";
    data.inputLength = strlen(data.inputData) + 1;
    printf("Print message in debug from driver\n");
    ioctl(device, IOCTL_BLOCK_DRV_SET, &data);
    ioctl(device, IOCTL_BLOCK_DRV_DBG_MESSAGE, &data);
    
    data.inputData = "New message to see...";
    data.inputLength = strlen(data.inputData) + 1;
    data.outputData = tempBuffer;
    data.outputLength = sizeof(tempBuffer);
    ioctl(device, IOCTL_BLOCK_DRV_GET_AND_SET, &data);
    printf("Get old message from driver and send new:\n");
    printf("MSG: %s\n", data.outputData);

    data.outputData = tempBuffer;
    data.outputLength = sizeof(tempBuffer);
    ioctl(device, IOCTL_BLOCK_DRV_GET, &data);
    printf("Get new message from driver:\n");
    printf("MSG: %s\n", data.outputData);

    close(device);
}