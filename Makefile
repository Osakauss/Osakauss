KERNEL := kernel
ISO_IMAGE = disk.iso
DISK=Primum/disk.img
FIRMWARE=legacy
CC := gcc
AS=nasm
LD=ld
OBJCOPY = objcopy
SRCDIR=src
BUILDDIR=bin
CFLAGS = -m64 -Wall -Wextra -Werror -O2 -pipe -g  -std=gnu99
ASFLAGS =-f elf64 -I$(SRCDIR)include/assembly
INTERNALLDFLAGS := \
	-m64 		\
	-nostdlib -static -Bsymbolic -no-pie -fno-pic -z max-page-size=0x1000 \
	-T$(SRCDIR)/kernel/linker.ld    \


INTERNALCFLAGS  :=       \
	-I $(SRCDIR)/include \
	-m64				\
	-nostdlib -ffreestanding -mcmodel=kernel -fno-builtin -fno-stack-protector -fno-pic -mno-80387 -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone

CFILES  = $(shell find $(SRCDIR) -name '*.c')
#OBJS   = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(COBJS))
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CFILES))
ASMFILES  = $(shell find $(SRCDIR) -name '*.s')
OBJ  += $(patsubst $(SRCDIR)/%.s,$(BUILDDIR)/%.s.o,$(ASMFILES))



.PHONY: all clean dirs

all: build

build:	dirs Primum-build $(KERNEL)


$(KERNEL): $(OBJ)
	@echo [$(CC)][LINKING ALL]
	@$(CC) $(INTERNALLDFLAGS) $(OBJ) -o $@


$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo [$(CC)][$<]
	@$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@
$(BUILDDIR)/%.s.o: $(SRCDIR)/%.s
	@echo [$(AS)][$<]
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	-@rm -rf $(BUILDDIR) $(ISO_IMAGE) $(KERNEL) disk
	-@cd Primum && make clean
	-@mdeltree -i $(DISK) ::
dirs:
	@mkdir -p $(BUILDDIR)
	@cd $(SRCDIR) \
	&& dirs=$$(find -type d) \
	&& cd ../$(BUILDDIR) \
	&& mkdir -p $$dirs


disk:  build
	-@mkdir -p disk
	-@mkdir -p disk/EFI && mkdir disk/EFI/Boot
	-@cp Primum/BOOTX64.EFI disk/EFI/Boot/
	-@cp kernel disk/
	-@cp meta/bootloader/Primum/config/primum.cfg disk/
	@python3 Primum/imgbuilder.py disk $(DISK) 


$(BUILDDIR)/kernel/kernel.dbg: $(KERNEL)
	@$(OBJCOPY) --only-keep-debug $< $@



Primum-build:
	@cd Primum && make


include meta/vm/qemu/*.mk
