#pragma once
#include <types.h>


static inline u8 
inb(u16 port)
{
  u8 data;

  asm volatile("in %1,%0" : "=a" (data) : "d" (port));
  return data;
}

static inline void 
insl(u16 port, void *addr, usize cnt)
{
  asm volatile("cld; rep insl" :
               "=D" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "memory", "cc");
}

static inline void 
outb(u16 port, u8 data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void 
outw(u16 port, u16 data)
{
  asm volatile("out %0,%1" : : "a" (data), "d" (port));
}

static inline void 
outsl(u16 port, const void *addr, usize cnt)
{
  asm volatile("cld; rep outsl" :
               "=S" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "cc");
}

static inline void 
sti()
{
  asm volatile("sti");
}
static inline void 
cli()
{
  asm volatile("cli");
}

static inline void 
enable_interrupts()
{
  sti();
}

static inline void 
hang(void)
{
	for (;;) ;
}


static inline void
reboot_system(){
  for (;;){
		int i;
		for (i = 0;i < 0x10000; i++){
			if ((inb(0x64) & 0x02) == 0){
				break;
			}

		}
		outb(0x64, 0xfe);
	}
}
static inline
void IOWait(){
  outb(0x80,0x00);
}

static inline u64 
rCR0() {
    u64 ret;
    __asm__ __volatile__("mov %%cr0, %0" : "=r"(ret));
    return ret;
}

static inline u64 
rCR2() {
    u64 ret;
    __asm__ __volatile__("mov %%cr2, %0" : "=r"(ret));
    return ret;
}

static inline u64 
rCR3() {
    u64 ret;
    __asm__ __volatile__("mov %%cr3, %0" : "=r"(ret));
    return ret;
}

static inline u64 
rCR4() {
    u64 ret;
    __asm__ __volatile__("mov %%cr4, %0" : "=r"(ret));
    return ret;
}

static inline u64 
rCR8() {
   u64 ret;
    __asm__ __volatile__("mov %%cr8, %0" : "=r"(ret));
    return ret;
}

static inline void 
wCR0(u64 val) {
    __asm__ __volatile__("mov %0, %%cr0" :: "r"(val));
}

static inline void 
wCR3(u64 val) {
    __asm__ __volatile__("mov %0, %%cr3" :: "r"(val));
}

static inline void 
wCR4(u64 val) {
    __asm__ __volatile__("mov %0, %%cr4" :: "r"(val));
}

static inline void 
wCR8(u64 val) {
    __asm__ __volatile__("mov %0, %%cr8" :: "r"(val));
}