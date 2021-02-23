# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct	s_data
{
	int			count;
	int			die_t;
	int			sleep_t;
	int			eat_t;
	int			max_eat;
}				t_data;

int	ft_isdigit(int c)
{
	if ((c >= '0') && (c <= '9'))
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
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

void	print_error(char *str)
{
	printf("%s", str);
	exit(0);
}

int		check_args(t_data *data)
{
	if (data->count < 2 || data->count > 200)
		return (0);
	if (data->die_t < 0)
		return (0);
	if (data->sleep_t < 0)
		return (0);
	if (data->eat_t < 0)
		return (0);
	if ((data->max_eat) && data->max_eat < 0)
		return (0);
	return (1);
}

int		parse_args(t_data *data, int ac, char **av)
{
	data->count = ft_atoi(av[1]);
	data->die_t = ft_atoi(av[2]);
	data->eat_t = ft_atoi(av[3]);
	data->sleep_t = ft_atoi(av[4]);
	if (av[5])
		data->max_eat = ft_atoi(av[5]);
	// else
	// 	data->max_eat = -1;
	if (check_args(data))
		return (1);
	else
		return (0);
}

int		main(int ac, char **av)
{
	t_data	data;

	if (ac != 5 && ac != 6)
		print_error("Error: wrong arguments!\n");
	// check digits
	if (!(parse_args(&data, ac, av)))
		print_error("Error: incorrect arguments!\n");

	return (0);
}

	// gettimeofday(&tv, NULL);
	// x = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
	// printf("x = %ld\n", x);
