#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

void err_show(void)
{
    printf("Error = %s\n", strerrorname_np(errno));
}

int main(void)
{

}