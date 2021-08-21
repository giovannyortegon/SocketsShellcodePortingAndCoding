/*
 *
 * thread2.c
 *
 */
#include<windows.h>
#include<stdio.h>
 
DWORD WINAPI thread_entry_point(LPVOID arg)
{
	printf("Thread 2!\n");
	return (0);
}
int main(void)
{
	HANDLE h = NULL;
	
	h = CreateThread(NULL, 0, thread_entry_point, NULL, 0, NULL);
	if (h == NULL)
	{
		printf("CreateThread() faild.\r\n");
		return(1);
	}
	Sleep(1000);
	printf("Thread 1!\r\n");
	
	return (0);
}
