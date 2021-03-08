/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:22:37 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/08 19:13:36 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		main(int ac, char **av)
{
	t_data		data;
	t_phil		*phil;
	t_semaphore	sem;

	if (ac != 5 && ac != 6)
		return (print_error("Wrong arguments!\n"));
	if (check_agrs(av))
		return (print_error("Arguments must be positive digit!\n"));
	if (!(parse_args(&data, av)))
		return (print_error("Incorrect arguments!\n"));
	if (!(phil = malloc(sizeof(t_phil) * data.count)))
		return (print_error("Malloc error!\n"));
	start_dinning(&data, phil, &sem);
	clear_after_dinning(phil, &sem);
	return (0);
}
