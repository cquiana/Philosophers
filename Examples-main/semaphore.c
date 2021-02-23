#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>

sem_t	*semaphore;

void	*draw(void *arg)
{
	sem_wait(semaphore);
	char ch = (char) arg;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			write(2, &ch, 1);
		write(2, "\n", 1);
	}
	sem_post(semaphore);
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
	semaphore = sem_open ("pSem", O_CREAT | O_EXCL, 0644, 3); 
	sem_unlink ("pSem");
	pthread_create(&thread1, NULL, draw, (void *) ch1);
	pthread_create(&thread2, NULL, draw, (void *) ch2);
	pthread_create(&thread3, NULL, draw, (void *) ch3);
	pthread_create(&thread4, NULL, draw, (void *) ch4);
	pthread_join(&thread1, NULL);
	pthread_join(&thread2, NULL);
	pthread_join(&thread3, NULL);
	pthread_join(&thread4, NULL);
	pthread_exit(NULL);
	return (0);
}