/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <cquiana@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:47:35 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 15:41:47 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	check_dead(t_phil *phil, long time)
{
	long	diff;

	diff = time - phil->last_eat_time;
	pthread_mutex_lock(&phil->data->dead_mutex);
	if ((phil->data->dead) || diff > phil->data->die_time)
	{
		pthread_mutex_unlock(&phil->data->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phil->data->dead_mutex);
	return (0);
}

int	check_max_eat(t_phil *phil)
{
	if (phil->data->max_eat == -1)
		return (0);
	pthread_mutex_lock(&phil->data->eat_mutex);
	if (phil->data->total_eat == phil->data->count)
	{
		pthread_mutex_unlock(&phil->data->eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phil->data->eat_mutex);
	return (0);
}

static void	print_status(t_phil *phil, long time)
{
	long	diff;

	diff = time - phil->data->start_time;
	if (phil->status.fork)
		printf("%10ld %d has taken a fork\n", diff, phil->id + 1);
	if (phil->status.eat)
	{
		printf("%10ld %d is eating\n", diff, phil->id + 1);
		phil->last_eat_time = time;
	}
	if (phil->status.sleep)
		printf("%10ld %d is sleeping\n", diff, phil->id + 1);
	if (phil->status.think)
		printf("%10ld %d is thinking\n", diff, phil->id + 1);
	if (phil->status.dead)
		printf("%10ld %d is dead\n", diff, phil->id + 1);
}

static void	reset_philo_status(t_phil *phil)
{
	phil->status.fork = FALSE;
	phil->status.eat = FALSE;
	phil->status.sleep = FALSE;
	phil->status.think = FALSE;
	phil->status.dead = FALSE;
}

int	display(t_phil *phil, long time)
{
	pthread_mutex_lock(&phil->data->print_mutex);
	if ((!(phil->status.dead) && check_dead(phil, time)) || check_max_eat(phil))
	{
		pthread_mutex_unlock(&phil->data->print_mutex);
		return (1);
	}
	print_status(phil, time);
	reset_philo_status(phil);
	pthread_mutex_unlock(&phil->data->print_mutex);
	return (0);
}
