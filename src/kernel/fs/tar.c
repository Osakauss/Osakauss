#include <libs/klibc.h>
#include <types.h>
#include <kernel/fs/tar.h>
#include <kernel/log.h>
#include <kernel/mem/pmm.h>

unsigned int getsize(const char *in)
{
 
    unsigned int size = 0;
    unsigned int j;
    unsigned int count = 1;
 
    for (j = 11; j > 0; j--, count *= 8)
        size += ((in[j - 1] - '0') * count);
 
    return size;
 
}
static int read(vfs_node* node, u32 size, u32 offset, char *buffer){
    (void)node;
    (void)size;
    (void)offset;
    (void)buffer;
    
    if ((u64)offset >= node->size){
        return 0;
    }
    int size_read = size;
    if ((u64)(offset+size) > node->size){
        size_read = node->size - (u64)offset;
    }
    vfs_read(node->device, size_read, node->offset+offset, buffer);
    return size_read;
}


vfs_node* tar_parse_filesystem(vfs_node *filesystem_disk)
{

    int offset = 0;
    struct tar_file_header header;
    vfs_node * root = (vfs_node*)pmm_calloc(sizeof(vfs_node));

    if (root == NULL){
        logf("Failed to get enough memory for root for tar filesystem\n");
        return NULL;
    }


    //strcpy(root->name, "initrd");
    root->children = (vfs_node**)pmm_calloc(sizeof(vfs_node));
    if (root->children == NULL){
        logf("Failed to get enough memory for tar filesystem\n");
        return NULL;
    }
    root->children_count = 0;
    root->type = VFS_NODE_DIRECTORY;


    vfs_node *file = NULL;
 
    while (1)
    {

        if (vfs_read(filesystem_disk, sizeof(struct tar_file_header), offset, (char*)&header) == 0){
            break;
        }
 
        if (header.name[0] == '\0')
            break;

        logf("tar file: %s\n", header.name);
 
        unsigned int size = getsize(header.size);


        file = (vfs_node*)pmm_calloc(sizeof(vfs_node));

        if (file == NULL){
            KERNEL_ERROR("Failed to get enough memory for new file");
        }

        file->size = size;
        strcpy(file->name, header.name);
        file->ops.read = read;
        file->offset = offset + sizeof(struct tar_file_header);
        file->type = VFS_NODE_FILE;
        file->device = filesystem_disk;

        vfs_add_node(root, file);
  
        offset += ((size / 512) + 1) * 512;
 
        if (size % 512)
            offset += 512;
 
    }
    return root;
 
}
