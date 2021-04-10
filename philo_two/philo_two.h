/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cquiana <cquiana@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 20:20:49 by cquiana           #+#    #+#             */
/*   Updated: 2021/04/10 15:56:10 by cquiana          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/time.h>

# define TRUE	1
# define FALSE	0

typedef struct s_data
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

typedef struct s_semaphore
{
	sem_t			*fork;
	sem_t			*hands;
	sem_t			*dead_sem;
	sem_t			*eat_sem;
	sem_t			*print_sem;
}					t_semaphore;

typedef struct s_status
{
	int				fork;
	int				eat;
	int				sleep;
	int				think;
	int				dead;
}					t_status;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals;
	long			last_eat_time;
	pthread_t		thread;
	t_data			*data;
	t_semaphore		*semaph;
	t_status		status;
}					t_phil;

int					ft_atoi(const char *str);
int					ft_isdigit(int c);
int					print_error(char *str);
int					check_agrs(char **av);
int					parse_args(t_data *data, char **av);
long				current_time(void);
void				start_dinning(t_data *data, t_phil *phil, t_semaphore *sem);
void				*symposium(void *args);
void				ft_mysleep(long time);
int					display(t_phil *phil, long time);
int					check_max_eat(t_phil *phil);
int					check_dead(t_phil *phil, long time);
void				unlink_sem(void);
void				clear_after_dinning(t_phil *phil);

#endif
