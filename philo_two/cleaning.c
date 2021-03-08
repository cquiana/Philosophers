/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 22:34:57 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 22:35:45 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	unlink_sem(void)
{
	sem_unlink("/forks_sem");
	sem_unlink("/hands_sem");
	sem_unlink("/eat_sem");
	sem_unlink("/dead_sem");
	sem_unlink("/print_sem");
}

void	clear_after_dinning(t_phil *phil)
{
	sem_close(phil->semaph->fork);
	sem_close(phil->semaph->eat_sem);
	sem_close(phil->semaph->dead_sem);
	sem_close(phil->semaph->print_sem);
	sem_close(phil->semaph->hands);
	free(phil);
}