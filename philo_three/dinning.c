/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinning.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 19:01:38 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 09:20:11 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static void	someone_dead(t_phil *phil, long time)
{
	if (phil->data->dead == 0)
	{
		phil->status.dead = TRUE;
		phil->data->dead = TRUE;
		display(phil, time);
		exit(1);
	}
}

static int	check_total_eat(t_phil *phil)
{
	if (phil->data->max_eat != -1 && phil->meals == phil->data->max_eat)
	{
		phil->data->total_eat++;
		return (1);
	}
	return (0);
}

static int	table(t_phil *phil)
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
	if (check_total_eat(phil))
		exit(2);
	return (0);
}

static void	*monitoring(void *agrs)
{
	t_phil	*phil;
	long	current;

	phil = (t_phil *)agrs;
	while (TRUE)
	{
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

void		*symposium(t_phil *phil)
{
	pthread_t	waiter;

	phil->last_eat_time = current_time();
	pthread_create(&waiter, NULL, monitoring, phil);
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
