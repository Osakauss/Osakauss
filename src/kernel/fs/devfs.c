#include <kernel/fs/devfs.h>
#include <types.h>
#include <kernel/fs/vfs.h>
#include <kernel/mem/pmm.h>
#include <libs/klibc.h>
#include <kernel/log.h>

typedef struct{
    char *data;
}devfs_file ;

static int files = 0;
devfs_file *devfs_root;


static void add_new_file(){
    devfs_root = pmm_realloc(devfs_root, sizeof(devfs_file) * files);
    devfs_root[files].data = pmm_alloc(100);
    files++;
}




int devfs_read(vfs_node *node, u32 size, u32 offset, char *buffer){
    memcpy(buffer, devfs_root[node->offset].data+offset, size);
    return offset;
}

int devfs_write(vfs_node *node , u32 size, u32 offset, char *data){
    if (node->size < offset+size){
        devfs_root[node->offset].data = pmm_realloc(devfs_root[node->offset].data, offset+size);
        node->size = offset+size;
    }
    memcpy(devfs_root[node->offset].data+offset, data, size);
    return offset;
}

int devfs_delfile(vfs_node *node){
    memset(devfs_root[node->offset].data, 0, node->size);
    return 1;
}   


int devfs_mkfile(vfs_node *parent, vfs_node *node, const char *name){
    UNUSED(parent);
    UNUSED(name);
    add_new_file();
    node->offset = files-1;
    node->ops.write = devfs_write;
    node->ops.read = devfs_read;
    return 1;
}



bool devfs_init(){

    vfs_node *root = vfs_get_node("/");

    vfs_node *dev = vfs_mkdir(root, "dev");

    devfs_root = pmm_alloc(sizeof(devfs_file));

    if(dev == NULL){
        return false;
    }
    
    dev->ops.mkfile = devfs_mkfile;
    dev->ops.delfile = devfs_delfile;

    vfs_mkfile(dev, "crap");



    return true;

    
}