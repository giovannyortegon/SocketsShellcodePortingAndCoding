/*
 * byteorder2.c
 */
#include<stdio.h>

int main(void)
{
// if WINE32, assume little endian
#ifdef WIN32
#define LITTLE_ENDIAN 1234
#define BYTE_ORDER LITTLE_ENDIAN
#endif
	return (0);
}
