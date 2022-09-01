#ifndef __DRIVER_FS_DEV_HEADER__
#define __DRIVER_FS_DEV_HEADER__
#include <types.h>
#include <kernel/fs/vfs.h>

bool devfs_init();
void add_new_device(int, char*, int (*write)(struct vfs_node *, u32, u32, char *), 
                    int (*read)(struct vfs_node *, u32, u32, char *));

#endif