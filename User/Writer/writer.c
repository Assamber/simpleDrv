#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>

#include "../../Drv/drv_ioctl.h"

#define DEVICE_NAME "/dev/DRiVeerDNA"
#define BUFFER_LEN 1024

#define SHARED_BUFFER_SIZE 1024
#define SHARED_BUFFER_NAME "myBuffer"
#define SEMAPHORE_NAME "mySemaphore"

int shMem = 0;
void* addr = NULL;
sem_t* sem = NULL;

void err_show(void)
{
    printf("Writer: Error = %s\n", strerrorname_np(errno));
}

void test_routine(int device)
{
    char tempBuffer[BUFFER_LEN] = {0};

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
}


int shared_mem_create(void)
{
    shMem = shm_open(SHARED_BUFFER_NAME, O_CREAT | O_RDWR, 0777);
    if(shMem < 0) return -1;
    ftruncate(shMem, SHARED_BUFFER_SIZE);
    if(shMem < 0)
    {
        return -1;
        shm_unlink(SHARED_BUFFER_NAME);
    }

    addr = mmap(NULL, SHARED_BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shMem, 0);
    if(addr == MAP_FAILED)
    {
        return -1;
        shm_unlink(SHARED_BUFFER_NAME);
    }

    return 0;
}

void shared_mem_close(void)
{
    shm_unlink(SHARED_BUFFER_NAME);
}

int start_viewer(void)
{
    pid_t id = fork();
    if(id < 0) 
    {
        printf("Failed run viewer!\n");
        err_show();
        return -1;
    }
    
    if(id == 0) //Child
    {
        execve("../Viewer/viewer", NULL, NULL);
    }

    return 0;
}

void shared_mem_write(void)
{
    sem_wait(sem);

    sem_post(sem);
}

int main(void)
{
    int err = 0;
    int status = 0;

    printf("Writer: has been started with ID = %d, with group = %d\n", getpid(), getpgid(0));

    // int device = open(DEVICE_NAME"0", O_RDWR);
    // if(device < 0)
    // {
    //     printf("Writer: Failed connect to driver!\n");
    //     err_show();
    //     return -ENODATA;
    // }

    //test_routine(device);

    err = shared_mem_create();
    if(err < 0)
    {
        printf("Writer: Failed create shared memory!\n");
        err_show();
        return -1;
    }

    sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 1);
    if (sem == SEM_FAILED)
    {
        printf("Writer: Failed create semaphore!\n");
        err_show();
        shared_mem_close();
        return -1;
    }

    printf("Writer: Start viewer...\n");
    start_viewer();
    signal(SIGURG, SIG_IGN);
    signal(SIGUSR1, SIG_IGN);

    char data[] = "1st step";
    memcpy(addr, data, sizeof(data));

    printf("Writer: Processes in the same group id\n");
    
    gets();
    kill(-getpgid(0), SIGURG);

    sleep(1);
    kill(-getpgid(0), SIGURG);

    sleep(1);
    kill(-getpgid(0), SIGUSR1);

    wait(&status);

    shared_mem_close();
    sem_close(sem);
    printf("Writer: Close...\n");
    //close(device);
}