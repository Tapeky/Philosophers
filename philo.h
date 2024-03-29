/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:30:53 by tsadouk           #+#    #+#             */
/*   Updated: 2024/02/02 13:53:41 by tsadouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdbool.h>


typedef enum e_philo_state
{
	EATING = 0,
	SLEEPING = 1,
	THINK = 2,
	DEAD = 3,
	FULL = 4,
	IDLE = 5	
}	t_state;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	struct s_data	*data;
	t_state			state;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	mut_state;
	pthread_mutex_t	mut_meals_eaten;
	pthread_mutex_t	mut_last_eat_time;
	u_int64_t		last_eat_time;
}               t_philo;


typedef struct s_data
{
	int				nb_philos;
	int				nb_meals;
	int				nb_full_p;
	bool			keep_iterating;
	u_int64_t		eat_time;
	u_int64_t		die_time;
	u_int64_t		sleep_time;
	u_int64_t		start_time;
	pthread_mutex_t	mut_eat_t;
	pthread_mutex_t	mut_die_t;
	pthread_mutex_t	mut_sleep_t;
	pthread_mutex_t	mut_print;
	pthread_mutex_t	mut_nb_philos;
	pthread_mutex_t	mut_keep_iter;
	pthread_mutex_t	mut_start_time;
	pthread_t		monit_all_alive;
	pthread_t		monit_all_full;
	pthread_t		*philo_ths;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}t_data;


void	*philo_routine(void *arg);
void	print_philo_action(t_philo *philosopher, const char *action);
void	change_philo_state(t_philo *philosopher, t_state new_state);
void	sleep_for(u_int64_t duration);
void init_all(t_data *data, char **argv);
void create_philo_threads(t_data *data);
void	init_philosophers(t_data *data);
void	init_forks(t_data *data);
void init_mutex(t_data *data);
void init_data(t_data *data, char **argv);
bool	errors_handeler(int argc);
void print_args(char **argv);
u_int64_t	get_time_in_ms(void);
int	ft_atoi(const char *str);
void	*monitor_all_alive(void *arg);

#endif