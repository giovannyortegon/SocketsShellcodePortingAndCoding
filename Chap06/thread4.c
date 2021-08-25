#include<windows.h>
#include<stdio.h>

// global vars
CRITICAL_SECTION lock;
int g_val = 0;

DWORD WINAPI thread_entry_point(LPVOID arg)
{
	while (1)
	{
		EnterCriticalSection(&lock);

		++g_val;
		printf("thread 2 , g_val: %d\n", g_val);

		LeaveCriticalSection(&lock);
		Sleep(1000);
	}		
}
int main(void)
{
	HANDLE h = NULL;
	int ret = 0;
	
	InitializeCriticalSection(&lock);
	
	h = CreateThread(NULL, 0, thread_entry_point, NULL, 0, NULL);
	if (h == NULL)
	{
		printf("CreateThread() failed.\r\n");
		return (1);
	}
	
	while (1)
	{
		EnterCriticalSection(&lock);
		
		++g_val;
		printf("thread 1 , g_val: %d\n", g_val);
		
		LeaveCriticalSection(&lock);
		Sleep(1000);
	}
	
	DeleteCriticalSection(&lock);
	
	return (0);
}
