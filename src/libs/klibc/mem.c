#include <types.h>
#include <libs/klibc.h>


void* memset(void* bufptr, u8 value, usize size) {
	unsigned char* buf = (unsigned char*) bufptr;
	for (usize i = 0; i < size; i++)
		buf[i] = (unsigned char) value;
	return bufptr;
}

void* memmove(void* dstptr, const void* srcptr, usize size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (usize i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (usize i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, usize size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (usize i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}


int memcmp(const void* aptr, const void* bptr, usize size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (usize i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}
