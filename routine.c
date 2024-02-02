/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:52:14 by tsadouk           #+#    #+#             */
/*   Updated: 2024/02/02 16:57:57 by tsadouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_philo_action(t_philo *philosopher, const char *action)
{
    pthread_mutex_lock(&philosopher->data->mut_print);
    
    if (philosopher->state == EATING)
        printf("\033[1;32mPhilosopher %d %s\033[0m\n", philosopher->id, action);
    else if (philosopher->state == SLEEPING)
        printf("\033[1;34mPhilosopher %d %s\033[0m\n", philosopher->id, action);
    else if (philosopher->state == THINK)
        printf("\033[1;33mPhilosopher %d %s\033[0m\n", philosopher->id, action);
    else if (philosopher->state == DEAD)
        printf("\033[1;31mPhilosopher %d %s\033[0m\n", philosopher->id, action);
    else if (philosopher->state == FULL)
        printf("\033[1;35mPhilosopher %d %s\033[0m\n", philosopher->id, action);
    else if (philosopher->state == IDLE)
        printf("\033[1;36mPhilosopher %d %s\033[0m\n", philosopher->id, action);
    else
        printf("Philosopher %d %s\n", philosopher->id, action);

    pthread_mutex_unlock(&philosopher->data->mut_print);
}


void	change_philo_state(t_philo *philosopher, t_state new_state)
{
	pthread_mutex_lock(&philosopher->mut_state);
	philosopher->state = new_state;
	pthread_mutex_unlock(&philosopher->mut_state);
}

void	sleep_for(u_int64_t duration)
{
	u_int64_t	start_time;
	u_int64_t	current_time;

	start_time = get_time_in_ms();
	while (1)
	{
		usleep(100);
		current_time = get_time_in_ms();
		if (current_time - start_time >= duration)
			break;
	}
}


void	*philo_routine(void *arg)
{
	t_philo *philosopher = (t_philo *)arg;
	t_data *data = philosopher->data;

	while (data->keep_iterating)
	{
		// Is thinking
		change_philo_state(philosopher, THINK);
		print_philo_action(philosopher, "is thinking");
		sleep_for(data->sleep_time);

		// Take a fork
		pthread_mutex_lock(philosopher->left_fork);
		change_philo_state(philosopher, EATING);
		print_philo_action(philosopher, "has taken a fork");
		pthread_mutex_lock(philosopher->right_fork);
		print_philo_action(philosopher, "has taken a fork");

		// Is eating
		print_philo_action(philosopher, "is eating");
		pthread_mutex_lock(&philosopher->mut_last_eat_time);
		philosopher->last_eat_time = get_time_in_ms();
		pthread_mutex_unlock(&philosopher->mut_last_eat_time);
		sleep_for(data->eat_time);

		// Put down the forks
		pthread_mutex_unlock(philosopher->left_fork);
		pthread_mutex_unlock(philosopher->right_fork);

		// Check if philosopher is full
		pthread_mutex_lock(&philosopher->mut_meals_eaten);
		philosopher->meals_eaten++;
		if (philosopher->meals_eaten >= data->nb_meals)
		{
			change_philo_state(philosopher, FULL);
			printf("Philosopher %d is full\n", philosopher->id);
			data->nb_full_p++;
			pthread_mutex_unlock(&philosopher->mut_meals_eaten);
			break;
		}
		pthread_mutex_unlock(&philosopher->mut_meals_eaten);

		// Is sleeping
		change_philo_state(philosopher, SLEEPING);
		print_philo_action(philosopher, "is sleeping");
		sleep_for(data->sleep_time);
	}

	return (NULL);
}


