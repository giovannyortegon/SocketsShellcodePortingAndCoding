/*
 * byteorder1.c
*/
#include<sys/endian.h>
#include<stdio.h>

int main(void)
{
#if BYTE_ORDER == LITTLE_ENDIAN
	printf("system is little endian!\n");
#elif BYTE_ORDER == BIG_ENDIAN
	printf("system is big endian.\n");
#else
	printf("not defined.\n");
#endif
	return (0);
}
