#ifndef __ISR_HEADER__
#define __ISR_HEADER__
#include <types.h>
#include <kernel/cpu/IRQ.h>
extern bool ISRInit();
void IRQSetMask(u8 IRQline);
 
void IRQClearMask(u8 IRQline);


extern void install_handler(int irq, void (*handler)(struct  regs *r));

extern void uninstall_handler(int irq);

#endif