/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <cquiana@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 23:13:28 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 15:36:16 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	unlink_sem(void)
{
	sem_unlink("/forks_sem");
	sem_unlink("/hands_sem");
	sem_unlink("/dead_sem");
	sem_unlink("/print_sem");
}

void	clear_after_dinning(t_phil *phil, t_semaphore *sem)
{
	sem_close(sem->fork);
	sem_close(sem->dead_sem);
	sem_close(sem->print_sem);
	sem_close(sem->hands);
	free(phil);
}

void	set_philo_status(t_phil *phil, int i)
{
	phil[i].status.fork = FALSE;
	phil[i].status.eat = FALSE;
	phil[i].status.sleep = FALSE;
	phil[i].status.think = FALSE;
	phil[i].status.dead = FALSE;
}
