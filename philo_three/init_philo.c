/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <cquiana@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 19:04:03 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 15:52:07 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	ft_kill(t_phil *phil)
{
	int	i;

	i = 0;
	while (i < phil->data->count)
	{
		kill(phil[i].pid, SIGKILL);
		i++;
	}
}

void	wait_and_kill(t_phil *phil)
{
	int	full;
	int	i;
	int	status;

	full = 0;
	while (TRUE)
	{
		i = 0;
		while (i++ < phil->data->count)
		{
			status = -1;
			waitpid(-1, &status, 0);
			if (status == 256)
				break ;
			if (status == 512)
			{
				if (++full != phil->data->count)
					continue ;
			}
		}
		if (status == 512 || full == phil->data->count || status == 256)
			ft_kill(phil);
		if (status == -1)
			break ;
	}
}

static void	create_procces(t_phil *phil)
{
	int		i;

	i = 0;
	while (i < phil->data->count)
	{
		phil[i].pid = fork();
		if (phil[i].pid == -1)
		{
			print_error("Fork error!\n");
			return ;
		}
		else if (phil[i].pid == 0)
		{
			symposium(&phil[i]);
			exit(0);
		}
		i++;
	}
	wait_and_kill(phil);
}

static int	init_semaphors(t_data *data, t_semaphore *sem)
{
	sem->fork = sem_open("/forks_sem", O_CREAT | O_EXCL, 0644, data->count);
	sem->hands = sem_open("/hands_sem", O_CREAT | O_EXCL,
			0644, data->count / 2);
	sem->dead_sem = sem_open("/dead_sem", O_CREAT | O_EXCL, 0644, 1);
	sem->print_sem = sem_open("/print_sem", O_CREAT | O_EXCL, 0644, 1);
	if (sem->print_sem == SEM_FAILED || sem->fork == SEM_FAILED
		|| sem->dead_sem == SEM_FAILED || sem->print_sem == SEM_FAILED)
		return (1);
	return (0);
}

void	start_dinning(t_data *data, t_phil *phil, t_semaphore *sem)
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
	create_procces(phil);
}
