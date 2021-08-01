QEMU=qemu-system-x86_64
QEMUOPTIONS=-drive format=raw,unit=0,file=$(DISK) -bios Primum/bios64.bin -m 256M -vga std -name Osakauss -machine q35 -serial stdio

run-qemu: disk
	@$(QEMU) $(QEMUOPTIONS)

run-qemu-dbg: disk $(BUILDDIR)/kernel/kernel.dbg
	$(QEMU) $(QEMUOPTIONS) -s -S &
	@sleep 1
	@gdb -x ./meta/vm/qemu/data/qemu.dbg