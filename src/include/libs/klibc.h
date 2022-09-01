#pragma once
#include <types.h>
#include <stdarg.h>



#define KERNEL_ERROR(msg) kernel_error(msg, __FILE__, __LINE__);
#define UNUSED(x) (void)(x)

extern void kernel_error (const char *msg, const char *file, u32 line);


/* libs/stdlib.c */
extern char *itoa(int value, char *str, int base);
extern u32 digit_count(int num);
extern usize strlen(const char* str);

extern int   memcmp(const void*, const void*, usize);
extern void *memcpy(void* __restrict, const void* __restrict, usize);
extern void *memmove(void*, const void*, usize);
extern void *memset(void*, u8, usize);

/* libs/format.c */
extern void format(void (*f)(char), const char *fmt, ...);
extern void formatv(void (*f)(char), const char *fmt, va_list args);


extern void strcpy(char * target, const char * src);
extern int strcmp(const char *str, const char *str1);
extern char *strdup(char *src);


extern int oct2bin(char *str, int size);

extern char *strtok(char *, char *);
