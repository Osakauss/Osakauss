#include <types.h>
#include <libs/klibc.h>
#include <kernel/log.h>
#include <x86.h>
#include <kernel/mem/pmm.h>

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
    char *s = pmm_alloc(100);
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





extern void kernel_error (const char *msg, const char *file, u32 line){
    __asm__ volatile("cli");
    logf("\n[ERROR] file:\04%s\01: line:\04%d\1 : \02%s\01\n", file,line,msg);
    hang();

}


int oct2bin(char *str, int size) {
    int n = 0;
    char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}