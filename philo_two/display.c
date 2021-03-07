/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 22:43:24 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 22:44:14 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int		check_dead(t_phil *phil, long time)
{
	long	diff;

	diff = time - phil->last_eat_time;
	sem_wait(phil->semaph->dead_sem);
	if ((phil->data->dead) || diff > phil->data->die_time)
	{
		sem_post(phil->semaph->dead_sem);
		return (1);
	}
	sem_post(phil->semaph->dead_sem);
	return (0);
}

int		check_max_eat(t_phil *phil)
{
	if (phil->data->max_eat == -1)
		return (0);
	sem_wait(phil->semaph->eat_sem);
	if (phil->data->total_eat == phil->data->count)
	{
		sem_post(phil->semaph->eat_sem);
		return (1);
	}
	sem_post(phil->semaph->eat_sem);
	return (0);
}

void	print_status(t_phil *phil, long time)
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
void	reset_philo_status(t_phil *phil)
{
	phil->status.fork = FALSE;
	phil->status.eat = FALSE;
	phil->status.sleep = FALSE;
	phil->status.think = FALSE;
	phil->status.dead = FALSE;
}

int		display(t_phil *phil, long time)
{
	sem_wait(phil->semaph->print_sem);
	if ((!(phil->status.dead) && check_dead(phil, time)) || check_max_eat(phil))
	{
		sem_post(phil->semaph->print_sem);
		return (1);
	}
	print_status(phil, time);
	reset_philo_status(phil);
	sem_post(phil->semaph->print_sem);
	return (0);
}
