/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 23:08:51 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 23:09:08 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		ft_isdigit(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	else
		return (0);
}

int		ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r'
		|| str[i] == '\f' || str[i] == '\v')
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

long	current_time(void)
{
	struct timeval	time;
	long			res;

	if ((gettimeofday(&time, NULL)) == -1)
		return (0);
	res = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (res);
}

void	ft_mysleep(long time)
{
	long	start;

	start = current_time();
	while ((current_time() - start) < time)
		usleep(100);
}

int		print_error(char *str)
{
	printf("%s", str);
	return (1);
}
