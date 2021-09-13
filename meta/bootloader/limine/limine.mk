LIMINE_INSTALLER=limine/limine-install-linux-x86_64


build_disk:  build
	mkdir $(ISO_DIR)/boot
	cp  meta/bootloader/limine/limine.cfg limine/limine.sys \
		limine/limine-cd.bin limine/limine-eltorito-efi.bin \
		$(ISO_DIR)/
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp $(RAMDISK) $(ISO_DIR)/boot/
ifeq ($(FIRMWARE),uefi)
	mkdir $(ISO_DIR)/EFI;
	mkdir $(ISO_DIR)/EFI/BOOT;
	cp ./limine/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT;
endif
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(ISO_IMAGE)

	$(LIMINE_INSTALLER) $(ISO_IMAGE)

	rm -rf $(ISO_DIR)
