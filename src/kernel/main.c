#include <stdint.h>
#include <lettre.h>
#include <x86.h>
#include <types.h>

#include <kernel/drivers/graphics/framebuffer.h>
#include <kernel/log.h>

#include <kernel/cpu/GDT.h>
#include <kernel/cpu/IDT.h>
#include <kernel/cpu/ISR.h>

#include <kernel/input.h>


bool init(LETTRE_PROTOCAL *lettre){


    framebufferInit(lettre->framebuffer.BaseAddress,lettre->framebuffer.ScreenWidth,
                    lettre->framebuffer.ScreenHeight,lettre->framebuffer.PixelsPerScanLine);


    require_log(LOG_BOTH);

    trace = true;

    logf("   ...:::   osakauss v0.0.0  :::...\n\n");

    
    gdtInit();
    IDTInit();
    ISRInit();
    enable_interrupts();
    require_input(INPUT_BOTH);
    //physmem_init();
    //mmInit(memmap);

    return true;
}




void main(LETTRE_PROTOCAL *lettre) {
    init(lettre);
    for (;;){
        asm volatile("hlt");
    }
}
