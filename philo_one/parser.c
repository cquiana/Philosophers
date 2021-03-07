/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:30:57 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 22:11:39 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int			check_agrs(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < 5)
	{
		j = -1;
		while (av[i][++j] != '\0')
		{
			if (!(ft_isdigit(av[i][j])))
				return (1);
		}
		i++;
	}
	if (av[5])
	{
		j = 0;
		while (av[5][j] != '\0')
		{
			if (!(ft_isdigit(av[i][j])))
				return (1);
			j++;
		}
	}
	return (0);
}

static int	validate_args(t_data *data)
{
	if (data->count < 2 || data->count > 200)
		return (0);
	if (data->die_time < 0)
		return (0);
	if (data->sleep_time < 0)
		return (0);
	if (data->eat_time < 0)
		return (0);
	return (1);
}

int			parse_args(t_data *data, char **av)
{
	data->count = ft_atoi(av[1]);
	data->die_time = (long)ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	if (av[5])
		data->max_eat = ft_atoi(av[5]);
	else
		data->max_eat = -1;
	if (validate_args(data))
		return (1);
	else
		return (0);
}
