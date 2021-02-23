#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


pthread_mutex_t	mutex;

void	*draw(void *arg)
{
	pthread_mutex_lock(&mutex);
	char ch = (char) arg;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			write(1, &ch, 1);
		write(1, "\n", 1);
	}
	pthread_mutex_unlock(&mutex);
	return (NULL);
}

int		main(void)
{
	pthread_t	thread1;
	pthread_t	thread2;
	pthread_t	thread3;
	pthread_t	thread4;
	char		ch1 = '1';
	char		ch2 = '2';
	char		ch3 = '3';
	char		ch4 = '4';
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&thread1, NULL, draw, (void *) ch1);
	pthread_create(&thread2, NULL, draw, (void *) ch2);
	pthread_create(&thread3, NULL, draw, (void *) ch3);
	pthread_create(&thread4, NULL, draw, (void *) ch4);
	pthread_exit(NULL);
	return (0);
}
