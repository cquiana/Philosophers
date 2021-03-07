/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 22:31:10 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 22:38:33 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

static void	create_threads(t_phil *phil)
{
	int		i;

	i = 0;
	while (i < phil->data->count)
	{
		phil[i].last_eat_time = current_time();
		if ((pthread_create(&phil[i].thread, NULL, symposium, &phil[i])) != 0)
			print_error("Thread create error!\n");
		i++;
	}
	i = 0;
	while (i < phil->data->count)
	{
		pthread_join(phil[i].thread, NULL);
		i++;
	}
}

static void	set_philo_status(t_phil *phil, int i)
{
	phil[i].status.fork = FALSE;
	phil[i].status.eat = FALSE;
	phil[i].status.sleep = FALSE;
	phil[i].status.think = FALSE;
	phil[i].status.dead = FALSE;
}

static int	init_semaphors(t_data *data, t_semaphore *sem)
{
	if ((sem->fork = sem_open("/forks_sem",
		O_CREAT | O_EXCL, 0644, data->count)) == SEM_FAILED)
		return (1);
	if ((sem->hands = sem_open("/hands_sem",
		O_CREAT | O_EXCL, 0644, data->count / 2)) == SEM_FAILED)
		return (1);
	if ((sem->eat_sem = sem_open("/eat_sem",
		O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		return (1);
	if ((sem->dead_sem = sem_open("/dead_sem",
		O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		return (1);
	if ((sem->print_sem = sem_open("/print_sem",
		O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		return (1);
	return (0);
}

void		start_dinning(t_data *data, t_phil *phil, t_semaphore *sem)
{
	int		i;

	i = 0;
	while (i < data->count)
	{
		phil[i].id = i;
		phil[i].meals = 0;
		set_philo_status(phil, i);
		phil[i].data = data;
		phil[i].semaph = sem;
		i++;
	}
	if (init_semaphors(data, sem))
	{
		free(phil);
		print_error("Semaphore error!\n");
	}
	unlink_sem();
	data->dead = 0;
	data->total_eat = 0;
	data->start_time = current_time();
	create_threads(phil);
}
