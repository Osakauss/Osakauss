#include <kernel/fs/devfs.h>
#include <kernel/mem/pmm.h>
#include <libs/klibc.h>
#include <kernel/log.h>

vfs_node *devfs_root;

void add_new_device(int device_id, char *name, int (*write)(struct vfs_node *, u32, u32, char *), 
                    int (*read)(struct vfs_node *, u32, u32, char *)){

    vfs_node* node = (vfs_node*)pmm_calloc(sizeof(vfs_node));
    strcpy(node->name, name);
    node->ops.write = write;
    node->ops.read = read;
    node->type = VFS_NODE_DEVICE;
    node->device_id = device_id;

    vfs_add_node(devfs_root, node);
}



bool devfs_init(){

    vfs_node *root = vfs_get_dir("/");

    vfs_node *dev_mountpoint = vfs_mkdir(root, "dev");

    vfs_node* dev_fs = pmm_calloc(sizeof(vfs_node));

    if(dev_mountpoint == NULL || dev_fs == NULL){
        return false;
    }

    dev_fs->children = (vfs_node**)pmm_calloc(sizeof(vfs_node));
    dev_fs->children_count = 0;
    dev_fs->type = VFS_NODE_DIRECTORY;
    strcpy(dev_fs->name, "dev");


    devfs_root = dev_fs;


    vfs_mount(dev_mountpoint, dev_fs);
    

    return true;

    
}