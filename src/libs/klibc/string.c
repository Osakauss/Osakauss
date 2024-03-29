#include <types.h>
#include <libs/klibc.h>
#include <kernel/mem/pmm.h>

usize strlen(const char* str)
{
	usize len = 0;
	while (str[len])
		len++;
	return len;
}

int strcmp(const char *str, const char *str1)
{
    for (int i = 0; ; i++)
    {
        if (str[i] != str1[i])
        {
            return str[i] < str1[i] ? -1 : 1;
        }
        if (str[i] == '\0')
        {
            return 0;
        }
    }
}




void strcpy(char * target, const char * src){
    u32 srclen = strlen(src);
    for (u32 v = 0; v <= srclen; v++){
        *target = *src;
        src++;
        target++;
    }
    return;
}



char *strdup(char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = pmm_alloc(len + 1);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}


static unsigned int is_delim(char c, char *delim)
{
    while(*delim != '\0')
    {
        if(c == *delim)
            return 1;
        delim++;
    }
    return 0;
}

char *strtok(char *srcString, char *delim)
{
    static char *backup_string;
    if(!srcString)
    {
        srcString = backup_string;
    }
    if(!srcString)
    {

        return NULL;
    }

    while(1)
    {
        if(is_delim(*srcString, delim))
        {
            srcString++;
            continue;
        }
        if(*srcString == '\0')
        {
            return NULL; 
        }
        break;
    }
    char *ret = srcString;
    while(1)
    {
        if(*srcString == '\0')
        {
            backup_string = srcString;
            return ret;
        }
        if(is_delim(*srcString, delim))
        {
            *srcString = '\0';
            backup_string = srcString + 1;
            return ret;
        }
        srcString++;
    }
}


