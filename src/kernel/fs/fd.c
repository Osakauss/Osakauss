#include <kernel/fs/fd.h>
#include <types.h>
#include <kernel/mem/pmm.h>
#include <libs/klibc.h>
#include <libs/dynlist.h>
#include <kernel/log.h>


dynlist fds;



void fd_init(){
    dl_init(&fds);
}


static int new_fd(vfs_node *node){
    fds.push(&fds,node);
    return fds.total(&fds)-1;
}


static void fd_remove(int fd){
    fds.set(&fds, fd, 0);
}




int fd_close(int fd){
    fd_remove(fd);
    return 1;
}

int fd_read(int fd, int size, int offset, char* data){
    return vfs_read(fds.get(&fds, fd), size, offset, data);
}

int fd_write(int fd, int size, int offset, char* data){
    return vfs_write(fds.get(&fds, fd), size, offset, data);
}


int fd_open(char *name){


    vfs_node *node = vfs_get_node(name);

    if (node == NULL){
        return -1;
    }

    return new_fd(node);;
}