#include <kernel/fs/vfs.h>
#include <kernel/mem/pmm.h>
#include <kernel/log.h>
#include <libs/klibc.h>
#include <kernel/fs/fd.h>



struct vfs_node* vfs_root;

vfs_node* vfs_get_root(){
    return vfs_root;
}


void vfs_init(){
    vfs_root = (vfs_node*)pmm_calloc(sizeof(vfs_node));
    vfs_root->children = (vfs_node**)pmm_calloc(sizeof(vfs_node));
    vfs_root->children_count = 0;
    vfs_root->type = VFS_NODE_DIRECTORY;
    strcpy(vfs_root->name, "/");
    vfs_root->del_flag = false;


    fd_init();

    return;
}

int vfs_read(vfs_node *node, u32 size, u32 offset, char *buffer){
    if (node->ops.read == 0){
        return NULL;
    }
    return node->ops.read(node, size, offset, buffer);
}

int vfs_write(vfs_node *node , int size, int offset, char *data){
    if (node->ops.write == 0){
        return NULL;
    }
    return node->ops.write(node, size, offset, data);
}



dynlist vfs_token_path(char *path){
    //logf("path: %s\n", path);

    dynlist t;
    dl_init(&t);

    char *name = strtok(path, "/");
    while (name != NULL) {
        t.push(&t, name);
        logf("name: %s\n", name);
        name = strtok(NULL, "/");
        
        
    }

    return t;
}


vfs_node *vfs_get_dir(char *name){
    dynlist path_tokens = vfs_token_path(name);

    /*for (int index = 0; index < path_tokens.total(&path_tokens); index++){
        logf("path:%d: %s\n", index, path_tokens.get(&path_tokens, index));
    }*/

    if (path_tokens.total(&path_tokens) == 0){
        return vfs_root;
    }

    vfs_node* node = vfs_root;

    bool found_child = false;

    for (int index = 0; index < path_tokens.total(&path_tokens); index++){
        for (u64 i = 0;i < node->children_count;i++ ){
            if (strcmp(node->children[i]->name, path_tokens.get(&path_tokens, index)) == 0){
                node = node->children[i];
                if (node->type == VFS_NODE_MOUNTPOINT){
                    node = node->ptr;
                }
                found_child = true;
                break;
            }
            else{
                found_child = false;
                continue;
            }
        }
        if (!found_child){
            return NULL;
        }
    }

    return node;
}



vfs_node* vfs_get_file(char* name){
    dynlist path_tokens = vfs_token_path(name);

    vfs_node* node = vfs_root;

    bool found_child = false;

    for (int index = 0; index < path_tokens.total(&path_tokens); index++){

        for (u64 i = 0;i < node->children_count;i++ ){
            if (strcmp(node->children[i]->name, path_tokens.get(&path_tokens, index)) == 0){
                node = node->children[i];

                if ( path_tokens.total(&path_tokens) == index){
                    if (node->type == VFS_NODE_FILE || node->type == VFS_NODE_DEVICE){
                        return node;
                    }
                    else{
                        return NULL;
                    }
                }else{
                    if (node->type == VFS_NODE_DIRECTORY);
                    else if (node->type == VFS_NODE_MOUNTPOINT){
                        node = node->ptr;
                    }
                    found_child = true;
                }
                
                break;
            }
            else{
                found_child = false;
                continue;
            }
        }
        if (!found_child){
            return NULL;
        }
    }

    return node;
}


vfs_node* vfs_mkdir(vfs_node *parent, char *name){
    
    if (parent->type == VFS_NODE_DIRECTORY && parent->del_flag != true){
         
        vfs_node *node = pmm_calloc(sizeof(vfs_node));
        if (node == NULL){
           
            return NULL;
        }

        node->children = (vfs_node**)pmm_calloc(sizeof(vfs_node));
        node->children_count = 0;
        node->type = VFS_NODE_DIRECTORY;  

        node->del_flag = false;

        strcpy(node->name, name);
        vfs_add_node(parent,node);
        if (parent->ops.mkdir != 0){
            parent->ops.mkdir(parent, node, name);
        }

        
        return node;
    }
    return NULL;
}
vfs_node* vfs_mkfile(vfs_node *parent, const char *name){
    if (parent->type == VFS_NODE_DIRECTORY && parent->del_flag != true){
        vfs_node *node = pmm_calloc(sizeof(vfs_node));
        if (node == NULL){
            return NULL;
        }
        node->del_flag = false;
        node->type = VFS_NODE_FILE;
        strcpy(node->name, name);
        vfs_add_node(parent,node);
        if (parent->ops.mkfile != 0){
            parent->ops.mkfile(parent, node, name);
        }
        return node;
    }
    return NULL;
}

int vfs_deldir(vfs_node *node){
    if (node->type == VFS_NODE_DIRECTORY){
        node->ops.deldir(node);
        pmm_free(node->children);
        pmm_free(node);
    }
    return 1;
}

int vfs_delfile(vfs_node *node){
    if (node->type == VFS_NODE_DIRECTORY){
        node->ops.delfile(node);
        pmm_free(node);
    }
    return 1;
}



int vfs_add_node(vfs_node *parent, vfs_node *node){
    if (parent->type != VFS_NODE_DIRECTORY){
        return NULL;
    }
    parent->children_count++;
    parent->children = pmm_realloc(parent->children, sizeof(parent->children)* parent->children_count);
    if (parent->children == NULL){
        KERNEL_ERROR("NOT ENOUGH RAM");
    }
    parent->children[parent->children_count-1] = node;
    node->child_id = parent->children_count-1;
    node->parent = parent;

    return 1;
}

void vfs_mount(vfs_node *mountpoint, vfs_node * filesystem){
    if (mountpoint == NULL){
        logf("Mount Point is NULL\n");
        return;
    }
    if (mountpoint->type == VFS_NODE_MOUNTPOINT){
        logf("Filesystem already been mounted there\n");
        return;
    }
    if (mountpoint->type != VFS_NODE_DIRECTORY){
        return;
    }
    
    mountpoint->type = VFS_NODE_MOUNTPOINT;
    mountpoint->ptr = filesystem;
}