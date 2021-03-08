/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 23:13:28 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 23:14:12 by cquiana          ###   ########.fr       */
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
