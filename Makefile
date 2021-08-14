KERNEL := kernel.elf
ISO_IMAGE = Osakauss.iso
ISO_DIR=disk
FIRMWARE=legacy

ifneq (,$(wildcard ./meta/toolchain/gcc/bin/x86_64-linux-gcc))
	CC := ./meta/toolchain/gcc/bin/x86_64-linux-gcc
endif

ifneq (,$(wildcard ./meta/toolchain/gcc/bin/x86_64-elf-gcc))
	CC := ./meta/toolchain/gcc/bin/x86_64-elf-gcc
endif

AS=nasm
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
OBJ = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CFILES))
ASMFILES  = $(shell find $(SRCDIR) -name '*.s')
OBJ  += $(patsubst $(SRCDIR)/%.s,$(BUILDDIR)/%.s.o,$(ASMFILES))



.PHONY: all clean dirs

all: build

build:	dirs $(KERNEL)


$(KERNEL): $(OBJ)
	@echo [$(CC)][LINKING ALL]
	@$(CC) $(INTERNALLDFLAGS) $(OBJ) -o $@


$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "[$(CC)]===>[$<]"
	@$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@
$(BUILDDIR)/%.s.o: $(SRCDIR)/%.s
	@echo "[$(AS)]===>[$<]"
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	-@rm -rf $(BUILDDIR) $(ISO_IMAGE) $(KERNEL) $(ISO_DIR)

dirs:
	-@mkdir -p $(ISO_DIR)
	@mkdir -p $(BUILDDIR)
	@cd $(SRCDIR) \
	&& dirs=$$(find -type d) \
	&& cd ../$(BUILDDIR) \
	&& mkdir -p $$dirs

$(BUILDDIR)/kernel/kernel.dbg: $(KERNEL)
	@$(OBJCOPY) --only-keep-debug $< $@

include meta/bootloader/limine/*.mk
include meta/vm/qemu/*.mk
include meta/toolchain/*.mk
