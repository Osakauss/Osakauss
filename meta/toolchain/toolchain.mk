# where compilers can be found
#https://mirrors.edge.kernel.org/pub/tools/crosstool/files/bin/x86_64/11.1.0/
# we use gcc 11.1.0, i like to use the latest software.

TARGET=x86_64-elf
CURRENT_PATH=$(shell pwd)
PREFIX=$(CURRENT_PATH)/meta/toolchain/gcc
THREAD_COUNT=6



build-toolchain-prebuilt:
	-@mkdir gcc-build
	@cd gcc-build && \
		wget https://mirrors.edge.kernel.org/pub/tools/crosstool/files/bin/x86_64/11.1.0/x86_64-gcc-11.1.0-nolibc-x86_64-linux.tar.gz && \
		tar -xf x86_64-gcc-11.1.0-nolibc-x86_64-linux.tar.gz && \
		cp -r gcc-11.1.0-nolibc/x86_64-linux ../meta/toolchain/gcc && \
		rm -rf gcc-11.1.0-nolibc && \
		rm -rf x86_64-gcc-11.1.0-nolibc-x86_64-linux.tar.gz
	@rm -rf gcc-build
build-toolchain:
		-@mkdir gcc-build
		@cd gcc-build && \
			wget https://ftp.gnu.org/gnu/gcc/gcc-11.1.0/gcc-11.1.0.tar.gz && \
			wget https://ftp.gnu.org/gnu/binutils/binutils-2.37.tar.gz && \
			tar -xzf binutils-2.37.tar.gz && \
			tar -xzf gcc-11.1.0.tar.gz
		-@mkdir gcc-build/build-binutils
		@cd gcc-build/build-binutils  && \
		    ../binutils-2.37/configure --target=$(TARGET) --prefix="$(PREFIX)" --with-sysroot --disable-nls --disable-werror && \
			make -j $(THREAD_COUNT) && \
			make install
		-@mkdir gcc-build/build-gcc
		@cd gcc-build && \
			cd build-gcc && \
			../gcc-11.1.0/configure --target=$(TARGET) --prefix="$(PREFIX)" --disable-nls --enable-languages=c,c++ --without-headers && \
			make -j $(THREAD_COUNT) all-gcc && \
			make -j $(THREAD_COUNT) all-target-libgcc && \
			make install-gcc && \
			make install-target-libgcc
		@rm -rf gcc-build
toolchain-remove:
		@rm -rf meta/toolchain/gcc
