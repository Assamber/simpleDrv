#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>

#define SHARED_BUFFER_SIZE 1024
#define SHARED_BUFFER_NAME "myBuffer"
#define SEMAPHORE_NAME "mySemaphore"

int shMem = 0;
void* addr = NULL;
sem_t* sem = NULL;
int cmd = 0;

void err_show(void)
{
    printf("Viewer: Error = %s\n", strerrorname_np(errno));
}


int shared_mem_open(void)
{
    shMem = shm_open(SHARED_BUFFER_NAME, O_RDWR, 0777);
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

void shared_mem_read(void)
{
    sem_wait(sem);
    printf("Viewer: Data = %s\n", (char *)addr);
    sem_post(sem);
}

int cmd_routune(void)
{
    switch (cmd)
    {
        case SIGURG: shared_mem_read(); break;
        case SIGUSR1: printf("Viewer: Cmd to stop...\n"); return 1;
        default: printf("Viewer: Cmd err!\n"); return -1;
    }

    return 0;
}

void signalHandler(int signum)
{
    cmd = signum;
}

int main(void)
{
    int err;
    printf("Viewer has been started with ID = %d, and group = %d\n", getpid(), getpgid(0));

    err = shared_mem_open();
    if(err < 0)
    {
        printf("Viewer: Failed create shared memory!\n");
        err_show();
        return -1;
    }

    sem = sem_open(SEMAPHORE_NAME, 0, 0777, 1);
    if (sem == SEM_FAILED)
    {
        printf("Writer: Failed create semaphore!\n");
        err_show();
        shared_mem_close();
        return -1;
    }

    signal(SIGURG, signalHandler);
    signal(SIGUSR1, signalHandler);

    while(1)
    {
        pause();
        printf("Viewer: Get new signal...\n");
        if(cmd_routune()) break;
    }

    printf("Viewer: Close...\n");
    //shared_mem_close();
}