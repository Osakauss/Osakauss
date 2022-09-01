#ifndef __FS_VFS_HEADER__
#define  __FS_VFS_HEADER__
#include <types.h>
#include <libs/dynlist.h>


enum node_type{
    VFS_NODE_FILE,
    VFS_NODE_DIRECTORY,
    VFS_NODE_MOUNTPOINT,
    VFS_NODE_DEVICE,
};


struct vfs_node;

typedef struct{
    int (*read) (struct vfs_node *, u32, u32, char *);
    int (*write) (struct vfs_node *, u32, u32, char *);
    void (*open) (struct vfs_node *, i32 flags);
    void (*close) (struct vfs_node *);
    int (*mkdir) (struct vfs_node *, struct vfs_node *,char *name);
    int (*mkfile) (struct vfs_node *, struct vfs_node *, const char *name);
    int (*delfile) (struct vfs_node*);
    int (*deldir) (struct vfs_node*);
}vfs_ops;

typedef struct vfs_node{
    char name[256];
    u32 mask;
    u32 gid;
    u32 uid;
    u64 size;
    u32 mtime; // modified time 
    u32 impl;
    enum node_type type;
    bool open_flags;
    bool del_flag;
    vfs_ops ops;
    u64 offset;
    struct vfs_node *parent;
    struct vfs_node **children;
    u64 children_count;
    u64 child_id;
    struct vfs_node *ptr;
    int device_id;
}vfs_node;

struct dirent{
    char name[256];
    u32 ino;
};


extern void vfs_init();
extern int vfs_add_node(vfs_node *parent, vfs_node *node);

extern vfs_node* vfs_mkfile(vfs_node *parent, const char *name);
extern vfs_node* vfs_mkdir(vfs_node *parent, char *name);

extern int vfs_write(vfs_node *node , int size, int offset, char *data);
extern int vfs_read(vfs_node *node, u32 size, u32 offset, char *buffer);

extern int vfs_deldir(vfs_node *node);

extern dynlist vfs_token_path(char *path);


extern vfs_node *vfs_get_dir(char *name);
extern vfs_node* vfs_get_file(char* name);
vfs_node* vfs_get_root();


void vfs_mount(vfs_node *, vfs_node *);

#endif
