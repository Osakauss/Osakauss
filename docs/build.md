# How to build Osakauss kernel

## You will need
 - gcc
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
make build
```
if you want to run and build it then use
```
make run-qemu
```
that will build and run the kernel
