/*
 * thread3.c
 */

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

// global variables
pthread_mutex_t lock;
int g_val = 0;

void *thread_entry_point(void *arg)
{
	while (1)
	{
		pthread_mutex_lock(&lock);

		++g_val;

		printf("thread 2, g_val: %d\n", g_val);

		pthread_mutex_unlock(&lock);
		usleep(1000000);
	}
}

int main(void)
{
	pthread_t pt;
	int ret = 0;

	ret = pthread_mutex_init(&lock, NULL);
	if (ret != 0x00)
	{
		printf("pthread_mutex_init() failed.\n");
		return (1);
	}

	ret = pthread_create(&pt, NULL, thread_entry_point, NULL);
	if (ret != 0x00)
	{
		printf("pthread_create() failed\n");
		return (1);
	}

	while (1)
	{
		pthread_mutex_lock(&lock);
		++g_val;
		printf("phread 1, g_val: %d\n", g_val);
		pthread_mutex_unlock(&lock);
		usleep(1000000);
	}

	pthread_mutex_destroy(&lock);

	return (0);
}
