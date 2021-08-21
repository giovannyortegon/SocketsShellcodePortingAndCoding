/*
 *
 * thread1.c
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *thread_entry_point(void *arg)
{
	printf("Thread 2!\n");
}

int main()
{
	pthread_t pt;
	int ret = 0;

	ret = pthread_create(&pt, NULL, &thread_entry_point, NULL);
	if (ret != 0x00)
	{
		printf("pthread_create() failed.\n");
		return (1);
	}

//	pthread_join(pt, NULL);

	sleep(1);

	printf("Thread 1!\n");

	return (0);
}
