/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <cquiana@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:22:30 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 15:56:58 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	main(int ac, char **av)
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
	phil = malloc(sizeof(t_phil) * data.count);
	if (!phil)
		return (print_error("Malloc error!\n"));
	start_dinning(&data, phil, &sem);
	clear_after_dinning(phil);
	return (0);
}
