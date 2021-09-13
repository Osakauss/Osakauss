#include <kernel/fs/vfs.h>
#include <kernel/mem/pmm.h>
#include <kernel/log.h>
#include <libs/klibc.h>
#include <kernel/fs/fd.h>



struct vfs_node* vfs_root;










void vfs_init(){
    vfs_root = (vfs_node*)pmm_calloc(sizeof(vfs_node));
    vfs_root->children = (vfs_node**)pmm_calloc(sizeof(vfs_node));
    vfs_root->children_count = 0;
    vfs_root->flag = 1;
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

    dynlist t;
    dl_init(&t);

    char *name = pmm_alloc(256);
    int nm_offset = 0;
    for (u32 v = 0; v < strlen(path)+1; v++){ // we add 1 so that we can get to the end
        if (path[v] == '/'){
            if (nm_offset != 0){
                t.push(&t, strdup(name));
            }
            t.push(&t, "/");
            memset(name, 0, 256);
            nm_offset = 0;
        }
        else if (path[v] == '\0'){
            if (nm_offset != 0){
                t.push(&t, name);
            }
            break;
        }
        else{
            name[nm_offset++] = path[v];
            
        }
    }
    pmm_free(name);
    return t;
}




vfs_node *vfs_get_node(char *name){
    dynlist pt = vfs_token_path(name);
    vfs_node *node = vfs_root;

    bool found_node = true;
    for (int u = 0; u <= pt.total(&pt); u++ ){
        if (found_node == false){
            return NULL;
        }
        if (u == 0){
            if (strcmp(pt.get(&pt, u), "/") == 0){ // all paths must start with root. If they do not then we turn NULL
                continue;
            }
            return NULL;
        }
        else if(strcmp(pt.get(&pt, u), "/") == 0){
            /*
                we must check if the node that we got is a directory  or something like this would be valid "/mydir/myfile.txt/" 
                which doesn't make sense because that would mean the file is a dir. 
                This  is why we check if it is a directory
            */
            if (node->flag != 1){ 
                return NULL;
            }
            if (u == 0){
                continue;
            }
            continue;
        }

        for (u64 i = 0;i < node->children_count;i++ ){
            if (strcmp(node->children[i]->name, pt.get(&pt, u)) == 0){
                node = node->children[i];
                found_node = true;
                break;
            }
            else{
                found_node = false;
            }
        }
        
    }
    return node;
    
}



vfs_node* vfs_mkdir(vfs_node *parent, char *name){
    
    if (parent->flag == 1 && parent->del_flag != true){
         
        vfs_node *node = pmm_calloc(sizeof(vfs_node));
        if (node == NULL){
           
            return NULL;
        }


        node->flag = 1;

        node->children = (vfs_node**)pmm_calloc(sizeof(vfs_node));
        node->children_count = 0;

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
    if (parent->flag == 1 && parent->del_flag != true){
        vfs_node *node = pmm_calloc(sizeof(vfs_node));
        if (node == NULL){
            return NULL;
        }
        node->flag = 0;
        node->del_flag = false;
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
    if (node->flag == 1){
        node->ops.deldir(node);
        pmm_free(node->children);
        pmm_free(node);
    }
    return 1;
}

int vfs_delfile(vfs_node *node){
    if (node->flag == 0){
        node->ops.delfile(node);
        pmm_free(node);
    }
    return 1;
}



int vfs_add_node(vfs_node *parent, vfs_node *node){
    if (parent->flag != 1){
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