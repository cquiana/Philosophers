/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/06 17:24:57 by cquiana           #+#    #+#             */
/*   Updated: 2021/03/07 19:14:22 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/time.h>
# include <signal.h>

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
}					t_data;

typedef struct		s_semaphore
{
	sem_t			*fork;
	sem_t			*hands;
	sem_t			*dead_sem;
	sem_t			*eat_sem;
	sem_t			*print_sem;
}					t_semaphore;

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
	pid_t			pid;
	t_data			*data;
	t_semaphore		*semaph;
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
	sem_wait(phil->semaph->dead_sem);
	if ((phil->data->dead) || diff > phil->data->die_time)
	{
		sem_post(phil->semaph->dead_sem);
		return (1);
	}
	sem_post(phil->semaph->dead_sem);
	return (0);
}

int		check_max_eat(t_phil *phil)
{
	if (phil->data->max_eat == -1)
		return (0);
	sem_wait(phil->semaph->eat_sem);
	if (phil->data->total_eat == phil->data->count)
	{
		sem_post(phil->semaph->eat_sem);
		return (1);
	}
	sem_post(phil->semaph->eat_sem);
	return (0);
}

int		print_status(t_phil *phil, long time)
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
	{
		printf("%10ld %d is dead\n", diff, phil->id + 1);
		return (1);
	}
	return (0);
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
	sem_wait(phil->semaph->print_sem);
	// if ((!(phil->status.dead) && check_dead(phil, time)) || check_max_eat(phil))

	if ((!(phil->status.dead) && check_dead(phil, time)))
	// if (phil->data->dead)
	{
		sem_post(phil->semaph->print_sem);
		return (1);
	}
	if (print_status(phil, time))
		return (1);
	reset_philo_status(phil);
	sem_post(phil->semaph->print_sem);
	return (0);
}

void	someone_dead(t_phil *phil, long time)
{
	if (phil->data->dead == 0)
	{
		phil->status.dead = TRUE;
		phil->data->dead = TRUE;
		display(phil, time);
		exit(1);
	}
}

void	*monitoring(void *agrs)
{
	t_phil	*phil;
	long	current;

	phil = (t_phil *)agrs;
	while (phil->meals != phil->data->max_eat)
	{
		// if (phil->meals == phil->data->max_eat)
		// 	break ;
		current = current_time();
		if (current - phil->last_eat_time > phil->data->die_time)
		{
			sem_wait(phil->semaph->dead_sem);
			someone_dead(phil, current);
			sem_post(phil->semaph->dead_sem);
			return (NULL);
		}
		ft_mysleep(1);
	}
	return (NULL);
}

int		check_total_eat(t_phil *phil)
{
	if (phil->meals == phil->data->max_eat)
	{
		phil->data->total_eat++;
		sem_post(phil->semaph->eat_sem);
		return (1);
	}
	return (0);
}

int		table(t_phil *phil)
{
	sem_wait(phil->semaph->hands);
	sem_wait(phil->semaph->fork);
	phil->status.fork = TRUE;
	display(phil, current_time());
	sem_wait(phil->semaph->fork);
	phil->status.fork = TRUE;
	display(phil, current_time());
	phil->status.eat = TRUE;
	display(phil, current_time());
	ft_mysleep(phil->data->eat_time);
	sem_post(phil->semaph->hands);
	sem_post(phil->semaph->fork);
	sem_post(phil->semaph->fork);
	phil->meals++;
	// sem_wait(phil->semaph->eat_sem);
	// if (check_total_eat(phil))
	// 	return (1);
	// sem_post(phil->semaph->eat_sem);
	return (0);
}

void	*symposium(t_phil *phil)
{
	pthread_t	waiter;

	phil->last_eat_time = current_time();
	pthread_create(&waiter, NULL, monitoring, phil);
	// while (TRUE)
	while (phil->meals != phil->data->max_eat)
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

void	create_procces(t_phil *phil)
{
	int		i;
	int		status;

	i = 0;
	while (i < phil->data->count)
	{
		if ((phil[i].pid = fork()) == -1)
		{
			print_error("Fork error!\n");
			return ;
		}
		else if (phil[i].pid == 0)
		{
			symposium(&phil[i]);
			exit (0);
		}
		// printf("pid = %d from child i = %d\n", phil[i].pid, i);
		i++;
	}
	waitpid(-1, &status, 0);
	i = 0;
	while (i < phil->data->count)
	{
		kill(phil[i].pid, SIGKILL);
		i++;
	}
	// printf("pid = %d from main i = %d\n", phil[i].pid, i);
}

void	set_philo_status(t_phil *phil, int i)
{
	phil[i].status.fork = FALSE;
	phil[i].status.eat = FALSE;
	phil[i].status.sleep = FALSE;
	phil[i].status.think = FALSE;
	phil[i].status.dead = FALSE;
}

int		init_semaphors(t_data *data, t_semaphore *sem)
{
	if ((sem->fork = sem_open("/forks_sem",
		O_CREAT | O_EXCL, 0644, data->count)) == SEM_FAILED)
		return (1);
	if ((sem->hands = sem_open("/hands_sem",
		O_CREAT | O_EXCL, 0644, data->count / 2)) == SEM_FAILED)
		return (1);
	if ((sem->eat_sem = sem_open("/eat_sem",
		O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		return (1);
	if ((sem->dead_sem = sem_open("/dead_sem",
		O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		return (1);
	if ((sem->print_sem = sem_open("/print_sem",
		O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED)
		return (1);
	return (0);
}

void	unlink_sem(void)
{
	sem_unlink("/forks_sem");
	sem_unlink("/hands_sem");
	sem_unlink("/eat_sem");
	sem_unlink("/dead_sem");
	sem_unlink("/print_sem");
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

void	clear_after_dinning(t_phil *phil, t_semaphore *sem)
{
	sem_close(sem->fork);
	sem_close(sem->eat_sem);
	sem_close(sem->dead_sem);
	sem_close(sem->print_sem);
	sem_close(sem->hands);
	free(phil);
}

int		main(int ac, char **av)
{
	t_data		data;
	t_phil		*phil;
	t_semaphore	sem;

	if (ac != 5 && ac != 6)
		print_error("Wrong arguments!\n");
	// check digits
	if (!(parse_args(&data, ac, av)))
		print_error("Incorrect arguments count!\n");
	if (!(phil = malloc(sizeof(t_phil) * data.count)))
		print_error("Malloc error!\n");
	start_dinning(&data, phil, &sem);
	clear_after_dinning(phil, &sem);

	// printf("%d %ld %d %d\n", data.count, data.die_time, data.eat_time, data.sleep_time);
	return (0);
}
