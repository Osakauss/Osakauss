# osakauss

Our SUPER AWESOME😎 Kernel and User Space System.


## How to build Osakauss kernel


# Building with gcc
## You will need
 - gcc
 - nasm
 - gnu make
## If you want to run it you will also need
 - qemu-system-x86_64

## Building the toolchain
Before starting you will need a toolchain to compile the kernel.
Follow this guide to get a toolchain working.
You can build or just use a prebuilt one.
If you want to use the prebuilt one the please use
```
make build-toolchain-prebuilt
```
If that did not work then you may have to compile your own.
Now to compile your own it will be a bit more complex, but don't worry it's not that bad.
First we need to install some stuff.
So install these packages
```
libgmp3-dev libmpfr-dev libisl-dev libcloog-isl-dev libmpc-dev texinfo
```
Once you have those, run this
```
make build-toolchain
```
## Get the code
  ```
  git clone --recursive https://github.com/Osakuass/Osakauss.git
 ```
## Building the kernel
```
make build
```
if you want to run and build it then use
```
make run-qemu
```
that will build and run the kernel

# Building with clang

## You will need
- clang
- nasm
- gnu make

## If you want to run it you will also need
 - qemu-system-x86_64

## Get the code
  ```
  git clone --recursive https://github.com/Osakuass/Osakauss.git
 ```
## Building the kernel
```
make TOOLCHAIN=clang build
```
if you want to run and build it then use
```
make TOOLCHAIN=clang run-qemu
```
that will build and run the kernel

