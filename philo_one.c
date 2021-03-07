/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 21:32:28 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 20:26:14 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

#define TRUE	1
#define FALSE	0

typedef struct		s_data
{
	int				count;
	long			die_time;
	int				sleep_time;
	int				eat_time;
	int				max_eat;
	int				dead;
	int				total_eat;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	print_mutex;
}					t_data;

typedef struct		s_status
{
	int				fork;
	int				eat;
	int				sleep;
	int				think;
	int				dead;
}					t_status;

typedef struct		s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals;
	long			last_eat_time;
	pthread_t		thread;
	t_data			*data;
	t_status		status;
}					t_phil;

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
}

int		check_args(t_data *data)
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

int		parse_args(t_data *data, int ac, char **av)
{
	data->count = ft_atoi(av[1]);
	data->die_time = (long)ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	if (av[5])
		data->max_eat = ft_atoi(av[5]);
	else
		data->max_eat = -1;
	if (check_args(data))
		return (1);
	else
		return (0);
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

void	init_forks(t_data *data)
{
	if (!(data->forks = malloc(sizeof(pthread_mutex_t) * data->count)))
		print_error("Malloc error!\n");
}

void	ft_mysleep(long time)
{
	long	start;

	start = current_time();
	while ((current_time() - start) < time)
		usleep(100);
}

int		check_dead(t_phil *phil, long time)
{
	long	diff;

	diff = time - phil->last_eat_time;
	pthread_mutex_lock(&phil->data->dead_mutex);
	if ((phil->data->dead) || diff > phil->data->die_time)
	{
		pthread_mutex_unlock(&phil->data->dead_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phil->data->dead_mutex);
	return (0);
}

int		check_max_eat(t_phil *phil)
{
	if (phil->data->max_eat == -1)
		return (0);
	pthread_mutex_lock(&phil->data->eat_mutex);
	if (phil->data->total_eat == phil->data->count)
	{
		pthread_mutex_unlock(&phil->data->eat_mutex);
		return (1);
	}
	pthread_mutex_unlock(&phil->data->eat_mutex);
	return (0);
}

void	print_status(t_phil *phil, long time)
{
	long	diff;

	diff = time - phil->data->start_time;
	if (phil->status.fork)
		printf("%10ld %d has taken a fork\n", diff, phil->id + 1);
	if (phil->status.eat)
	{
		printf("%10ld %d is eating\n", diff, phil->id + 1);
		phil->last_eat_time = time;
	}
	if (phil->status.sleep)
		printf("%10ld %d is sleeping\n", diff, phil->id + 1);
	if (phil->status.think)
		printf("%10ld %d is thinking\n", diff, phil->id + 1);
	if (phil->status.dead)
		printf("%10ld %d is dead\n", diff, phil->id + 1);
}
void	reset_philo_status(t_phil *phil)
{
	phil->status.fork = FALSE;
	phil->status.eat = FALSE;
	phil->status.sleep = FALSE;
	phil->status.think = FALSE;
	phil->status.dead = FALSE;
}

int		display(t_phil *phil, long time)
{
	pthread_mutex_lock(&phil->data->print_mutex);
	if ((!(phil->status.dead) && check_dead(phil, time)) || check_max_eat(phil))
	{
		pthread_mutex_unlock(&phil->data->print_mutex);
		return (1);
	}
	print_status(phil, time);
	reset_philo_status(phil);
	pthread_mutex_unlock(&phil->data->print_mutex);
	return (0);
}

void	someone_dead(t_phil *phil, long time)
{
	if (phil->data->dead == 0)
	{
		phil->status.dead = TRUE;
		display(phil, time);
		phil->data->dead++;
	}
}

void	*monitoring(void *agrs)
{
	t_phil	*phil;
	long	current;

	phil = (t_phil *)agrs;
	while (TRUE)
	{
		if (phil->meals == phil->data->max_eat)
			break ;
		current = current_time();
		if (current - phil->last_eat_time > phil->data->die_time)
		{
			pthread_mutex_lock(&phil->data->dead_mutex);
			someone_dead(phil, current);
			pthread_mutex_unlock(&phil->data->dead_mutex);
			return (NULL);
		}
		ft_mysleep(1);
	}
	return (NULL);
}

int		check_total_eat(t_phil *phil)
{
	if (phil->data->max_eat != -1 && phil->meals == phil->data->max_eat)
	{
		phil->data->total_eat++;
		pthread_mutex_unlock(&phil->data->eat_mutex);
		return (1);
	}
	return (0);
}

int		table(t_phil *phil)
{
	pthread_mutex_lock(&phil->data->forks[phil->left_fork]);
	phil->status.fork = TRUE;
	display(phil, current_time());
	pthread_mutex_lock(&phil->data->forks[phil->right_fork]);
	phil->status.fork = TRUE;
	display(phil, current_time());
	phil->status.eat = TRUE;
	display(phil, current_time());
	ft_mysleep(phil->data->eat_time);
	pthread_mutex_unlock(&phil->data->forks[phil->left_fork]);
	pthread_mutex_unlock(&phil->data->forks[phil->right_fork]);
	phil->meals++;
	pthread_mutex_lock(&phil->data->eat_mutex);
	if (check_total_eat(phil))
		return (1);
	pthread_mutex_unlock(&phil->data->eat_mutex);
	return (0);
}

void	*symposium(void *args)
{
	t_phil		*phil;
	pthread_t	waiter;

	phil = (t_phil *)args;
	if (phil->id % 2 == 0)
		ft_mysleep(1);
	pthread_create(&waiter, NULL, monitoring, phil);
	while (TRUE)
	{
		if (table(phil))
			break ;
		phil->status.sleep = TRUE;
		if (display(phil, current_time()))
			break ;
		ft_mysleep(phil->data->sleep_time);
		phil->status.think = TRUE;
		if (display(phil, current_time()))
			break ;
	}
	pthread_join(waiter, NULL);
	return (NULL);
}

void	create_threads(t_phil *phil)
{
	int		i;

	i = 0;
	while (i < phil->data->count)
	{
		phil[i].last_eat_time = current_time();
		pthread_create(&phil[i].thread, NULL, symposium, &phil[i]);
		i++;
	}
	i = 0;
	while (i < phil->data->count)
	{
		pthread_join(phil[i].thread, NULL);
		i++;
	}
}

void	set_philo_status(t_phil *phil, int i)
{
	phil[i].status.fork = FALSE;
	phil[i].status.eat = FALSE;
	phil[i].status.sleep = FALSE;
	phil[i].status.think = FALSE;
	phil[i].status.dead = FALSE;
}

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->dead_mutex, NULL);
	pthread_mutex_init(&data->eat_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
}

void	start_dinning(t_data *data, t_phil *phil)
{
	int		i;

	i = 0;
	init_forks(data);
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
	init_mutex(data);
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

int		main(int ac, char **av)
{
	t_data	data;
	t_phil	*phil;

	if (ac != 5 && ac != 6)
		print_error("Wrong arguments!\n");
	// check digits
	if (!(parse_args(&data, ac, av)))
		print_error("Incorrect arguments count!\n");
	if (!(phil = malloc(sizeof(t_phil) * data.count)))
		print_error("Malloc error!\n");
	start_dinning(&data, phil);
	clear_after_dinning(&data, phil);
	return (0);
}

	// gettimeofday(&tv, NULL);
	// x = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
	// printf("x = %ld\n", x);
