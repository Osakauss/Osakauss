#ifndef __DRIVER_FS_TAR_HEADER__
#define __DRIVER_FS_TAR_HEADER__




struct tar_file_header{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12]; // modification date
    char checksum[8];
    char flag;
    char linkname[100];
    char ustar[6];
    char ustarv[2]; // ustar version
    char ownerun[32]; // file owner user name
    char ownergn[32]; // file owner group name
    char dvcmajorn[8]; // device major number
    char dvcminorn[8]; // device minor number;
    char filenameprefix[155];
    char NOTHING[12];
} __attribute__((packed));



#endif