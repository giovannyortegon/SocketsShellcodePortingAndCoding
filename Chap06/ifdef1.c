/*
 * ifdef1.c
 *
 * ifdef example program.
 */
#include<stdio.h>

int main(void)
{
#ifdef __linux
	/* print out if compiled on LINUX */
	printf("LINUX\n");
#elif	WIN32
	/* print out if compiled on Win32 */
	printf("WIN32\n");
#else
	printf("?\n");
#endif
	return (0);
}
