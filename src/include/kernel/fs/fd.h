#ifndef __FD_HEADER__
#define __FD_HEADER__
#include <kernel/fs/vfs.h>

typedef struct{
    vfs_node *node;
}fd_t;



extern void fd_init();
int fd_open(char *name);
int fd_close(int fd);
int fd_read(int fd, int size, int offset, char* data);
int fd_write(int fd, int size, int offset, char* data);


#endif