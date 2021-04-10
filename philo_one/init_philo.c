/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <cquiana@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:33:46 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 15:42:53 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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

static void	init_mutex(t_data *data)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->count);
	if (!data->forks)
		print_error("Malloc error!\n");
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->eat_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
}

void	start_dinning(t_data *data, t_phil *phil)
{
	int		i;

	i = 0;
	init_mutex(data);
	while (i < data->count)
	{
		phil[i].id = i;
		phil[i].meals = 0;
		set_philo_status(phil, i);
		if (i == 0)
			phil[i].left_fork = data->count - 1;
		else
			phil[i].left_fork = i - 1;
		phil[i].right_fork = i;
		pthread_mutex_init(&data->forks[i], NULL);
		phil[i].data = data;
		i++;
	}
	data->dead = 0;
	data->total_eat = 0;
	data->start_time = current_time();
	create_threads(phil);
}

void	clear_after_dinning(t_data *data, t_phil *phil)
{
	int		i;

	i = 0;
	while (i < data->count)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->dead_mutex);
	pthread_mutex_destroy(&data->eat_mutex);
	pthread_mutex_destroy(&data->print_mutex);
	free(data->forks);
	free(phil);
}
