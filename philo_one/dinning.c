/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinning.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:37:24 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 21:34:24 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static void	someone_dead(t_phil *phil, long time)
{
	if (phil->data->dead == 0)
	{
		phil->status.dead = TRUE;
		display(phil, time);
		phil->data->dead++;
	}
}

static int	check_total_eat(t_phil *phil)
{
	if (phil->data->max_eat != -1 && phil->meals == phil->data->max_eat)
	{
		phil->data->total_eat++;
		pthread_mutex_unlock(&phil->data->eat_mutex);
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
			pthread_mutex_lock(&phil->data->dead_mutex);
			someone_dead(phil, current);
			pthread_mutex_unlock(&phil->data->dead_mutex);
			return (NULL);
		}
		ft_mysleep(1);
	}
	return (NULL);
}

static int	table(t_phil *phil)
{
	pthread_mutex_lock(&phil->data->forks[phil->left_fork]);
	phil->status.fork = TRUE;
	display(phil, current_time());
	pthread_mutex_lock(&phil->data->forks[phil->right_fork]);
	phil->status.fork = TRUE;
	display(phil, current_time());
	phil->status.eat = TRUE;
	display(phil, current_time());
	ft_mysleep(phil->data->eat_time);
	pthread_mutex_unlock(&phil->data->forks[phil->left_fork]);
	pthread_mutex_unlock(&phil->data->forks[phil->right_fork]);
	phil->meals++;
	pthread_mutex_lock(&phil->data->eat_mutex);
	if (check_total_eat(phil))
		return (1);
	pthread_mutex_unlock(&phil->data->eat_mutex);
	return (0);
}

void		*symposium(void *args)
{
	t_phil		*phil;
	pthread_t	waiter;

	phil = (t_phil *)args;
	if (phil->id % 2 == 0)
		ft_mysleep(1);
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
