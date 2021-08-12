LIMINE_INSTALLER=limine/limine-install-linux-x86_64


build_disk:  build
	cp  $(KERNEL)  \
		meta/bootloader/limine/limine.cfg limine/limine.sys \
		limine/limine-cd.bin limine/limine-eltorito-efi.bin \
		$(ISO_DIR)/
ifeq ($(FIRMWARE),uefi)
	mkdir iso_root/EFI;
	mkdir iso_root/EFI/BOOT;
	cp limine/BOOTX64.EFI iso_root/EFI/BOOT;
endif
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(ISO_IMAGE)

	$(LIMINE_INSTALLER) $(ISO_IMAGE)

	rm -rf $(ISO_DIR)