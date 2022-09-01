QEMU=qemu-system-x86_64
QEMUOPTIONS=-m 256M -serial mon:stdio -cdrom $(ISO_IMAGE) 

run-qemu: build_disk
	@$(QEMU) $(QEMUOPTIONS)

run-qemu-dbg: build_disk $(BUILDDIR)/kernel/kernel.dbg
	$(QEMU) $(QEMUOPTIONS) -s -S &
	@sleep 1
	@gdb -x ./meta/vm/qemu/dbg/qemu.dbg