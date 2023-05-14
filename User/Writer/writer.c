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
#define VIEWER_PATH "../Viewer/viewer"
#define BUFFER_LEN 1024

#define SHARED_BUFFER_SIZE 1024
#define SHARED_BUFFER_NAME "myBuffer"
#define SEMAPHORE_NAME "mySemaphore"
#define MAXIMUM_TICKS 250

#define SIGNAL_SEND_DATA(groupID)     kill(-groupID, SIGURG)
#define SIGNAL_SEND_STOP(groupID)     kill(-groupID, SIGUSR1)

int shMem = 0;
void* addrShMem = NULL;
sem_t* sem = NULL;
volatile int sigBeen = 0;
char * const cmd[] = {VIEWER_PATH, NULL};

void err_show(void)
{
    printf("Writer: Error = %s\n", strerrorname_np(errno));
}

void signalHandler(int signum)
{
    sigBeen = 1;
    return;
}

int signalWait(void)
{
    sigBeen = 0;
    int ticks = 0;
    while(1)
    {
        if(sigBeen) break;
        ticks++;
        if(ticks >= MAXIMUM_TICKS) return -1;
        usleep(1000);
    }

    return 0;
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

    addrShMem = mmap(NULL, SHARED_BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shMem, 0);
    if(addrShMem == MAP_FAILED)
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
    int err = 0;

    pid_t id = fork();
    if(id < 0) 
    {
        printf("Writer: Failed run viewer!\n");
        err_show();
        return -1;
    }
    
    if(id == 0) //Child
    {

        err = execve(VIEWER_PATH, cmd, NULL);
        if(err < 0)
        {
            printf("Child: Can not run viewer, so sorry!\n");
            exit(-1);
        }
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
    
    signal(SIGURG, SIG_IGN);
    signal(SIGUSR1, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);

    printf("Writer: has been started with ID = %d, with group = %d\n", getpid(), getpgid(0));

    //Init Device, Semaphore and shared memory

    int device = open(DEVICE_NAME"0", O_RDWR);
    if(device < 0)
    {
        printf("Writer: Failed connect to driver!\n");
        err_show();
        return -ENODATA;
    }

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
    printf("Writer: Processes in the same group id\n");

    pid_t pgID = getpgid(0);

    //TEST ROUTINE
    size_t returnedSize = 0;
    simpleDrv_ioctl_data_t data = {0};
    data.returnedSize = &returnedSize;
    printf("Writer: Start test routine...\n");

    data.inputData = "Set this message to driver buffer!";
    data.inputLength = strlen(data.inputData) + 1;
    printf("Writer: Print message in debug from driver\n");
    ioctl(device, IOCTL_BLOCK_DRV_SET, &data);
    ioctl(device, IOCTL_BLOCK_DRV_DBG_MESSAGE, &data);

    signalWait();
    data.inputData = "New message to see...";
    data.inputLength = strlen(data.inputData) + 1;
    data.outputData = addrShMem;
    data.outputLength = SHARED_BUFFER_SIZE;
    ioctl(device, IOCTL_BLOCK_DRV_GET_AND_SET, &data);
    printf("Writer: Get old message from driver and send new.\n");
    SIGNAL_SEND_DATA(pgID);

    signalWait();
    data.outputData = addrShMem;
    data.outputLength = SHARED_BUFFER_SIZE;
    ioctl(device, IOCTL_BLOCK_DRV_GET, &data);
    printf("Writer: Get new message from driver.\n");
    SIGNAL_SEND_DATA(pgID);
    //END TEST ROUTUNE
    printf("Writer: Test routine end...\n");
    signalWait();
    SIGNAL_SEND_STOP(pgID);
    wait(&status);

    shared_mem_close();
    sem_close(sem);
    close(device);
    printf("Writer: Close...\n");
}