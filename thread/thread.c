#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;

void * hello(void *arg)
{
	int *num =(int *) arg;
	pthread_mutex_lock(&mutex);
	printf("Hello, %d\n",*num);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);

}

int main()
{

	int status = pthread_mutex_init(&mutex, NULL);
	if (status != 0) printf("Error: %s\n", strerror(status));
	int i, k[5];
	pthread_t thread[5];

	for (i = 0; i < 5; i++)
	{
		k[i] = i+1;
		pthread_create(&thread[i], NULL, hello, k + i);
	}

	for (i = 0; i < 5; i++)
	{
		pthread_join(thread[i], NULL);
	}


	pthread_mutex_destroy(&mutex);
	printf("Well Done Trico...\n");
}
