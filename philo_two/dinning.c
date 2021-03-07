/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinning.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 22:29:35 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 22:41:42 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	someone_dead(t_phil *phil, long time)
{
	if (phil->data->dead == 0)
	{
		phil->status.dead = TRUE;
		display(phil, time);
		phil->data->dead = TRUE;
	}
}

int		check_total_eat(t_phil *phil)
{
	if (phil->data->max_eat != -1 && phil->meals == phil->data->max_eat)
	{
		phil->data->total_eat++;
		sem_post(phil->semaph->eat_sem);
		return (1);
	}
	return (0);
}

static void	*monitoring(void *agrs)
{
	t_phil	*phil;
	long	current;

	phil = (t_phil *)agrs;
	while (TRUE)
	{
		if (phil->meals == phil->data->max_eat)
			break ;
		current = current_time();
		if (current - phil->last_eat_time > phil->data->die_time)
		{
			sem_wait(phil->semaph->dead_sem);
			someone_dead(phil, current);
			sem_post(phil->semaph->dead_sem);
			return (NULL);
		}
		ft_mysleep(1);
	}
	return (NULL);
}

int		table(t_phil *phil)
{
	sem_wait(phil->semaph->hands);
	sem_wait(phil->semaph->fork);
	phil->status.fork = TRUE;
	display(phil, current_time());
	sem_wait(phil->semaph->fork);
	phil->status.fork = TRUE;
	display(phil, current_time());
	phil->status.eat = TRUE;
	display(phil, current_time());
	ft_mysleep(phil->data->eat_time);
	sem_post(phil->semaph->hands);
	sem_post(phil->semaph->fork);
	sem_post(phil->semaph->fork);
	phil->meals++;
	sem_wait(phil->semaph->eat_sem);
	if (check_total_eat(phil))
		return (1);
	sem_post(phil->semaph->eat_sem);
	return (0);
}

void	*symposium(void *args)
{
	t_phil		*phil;
	pthread_t	waiter;

	phil = (t_phil *)args;
	if ((pthread_create(&waiter, NULL, monitoring, phil)) != 0)
		print_error("Thread create error!\n");
	while (TRUE)
	{
		if (table(phil))
			break ;
		phil->status.sleep = TRUE;
		if (display(phil, current_time()))
			break ;
		ft_mysleep(phil->data->sleep_time);
		phil->status.think = TRUE;
		if (display(phil, current_time()))
			break ;
	}
	pthread_join(waiter, NULL);
	return (NULL);
}
