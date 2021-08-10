QEMU=qemu-system-x86_64
QEMUOPTIONS=-m 256M -serial stdio -cdrom $(ISO_IMAGE) -no-reboot -no-shutdown -d int

run-qemu: build_disk
	@$(QEMU) $(QEMUOPTIONS)

run-qemu-dbg: build_disk $(BUILDDIR)/kernel/kernel.dbg
	$(QEMU) $(QEMUOPTIONS) -s -S &
	@sleep 1
	@gdb -x ./meta/vm/qemu/data/qemu.dbg
