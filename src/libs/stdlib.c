#include <types.h>
#include <libs/stdlib.h>
#include <kernel/log.h>
#include <x86.h>

char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

void reverse(char *s)
{
    int i,j,c;

    for(i=0,j=strlen(s)-1;i<j;i++,j--)
        c=s[i],s[i]=s[j],s[j]=c;
}

char *  itob(int n,  int b){
    char *s = s;
    int i,j,sign;

    if((sign=n)<0)
        n = -n;

    i = 0;

    do
    {
        j = n % b;

        s[i++] = (j <= 9)?j+'0':j+'a'-10;
    }while((n/=b)>0);

    if(sign < 0)
        s[i++]='-';

    s[i]='\0';

    reverse(s);
    return s;
}

usize strlen(const char* str)
{
	usize len = 0;
	while (str[len])
		len++;
	return len;
}


u32 digit_count(int num)
{
  u32 count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}



void* memset(void* bufptr, u8 value, usize size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (usize i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

void* memmove(void* dstptr, const void* srcptr, usize size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (usize i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (usize i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, usize size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (usize i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}


int memcmp(const void* aptr, const void* bptr, usize size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (usize i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

/*
char *strdup(char *src)
{
    char *str;
    char *p;
    int len = 0;

    while (src[len])
        len++;
    str = malloc(len + 1);
    p = str;
    while (*src)
        *p++ = *src++;
    *p = '\0';
    return str;
}*/

extern void kernel_error (const char *msg, const char *file, u32 line){
    __asm__ volatile("cli");
    logf("\n[ERROR] file:\04%s\01: line:\04%d\1 : \02%s\01\n", file,line,msg);
    hang();

}
