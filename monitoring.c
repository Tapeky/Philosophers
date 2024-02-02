/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 16:57:31 by tsadouk           #+#    #+#             */
/*   Updated: 2024/02/02 16:58:18 by tsadouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	*monitor_all_alive(void *arg)
{
	t_data *data;
	u_int64_t current_time;

	data = (t_data *)arg;
	while (1)
	{
		// Vérifier si chaque philosophe est en vie
		for (int i = 0; i < data->nb_philos; ++i)
		{
			pthread_mutex_lock(&data->philos[i].mut_last_eat_time);
			current_time = get_time_in_ms();
			if (current_time - data->philos[i].last_eat_time > (u_int64_t)data->die_time)
			{
				// Philosopher i is dead
				print_philo_action(&data->philos[i], "has died 💀");
				pthread_mutex_unlock(&data->philos[i].mut_last_eat_time);
				// TODO: Gérer la fin du programme en cas de décès
				exit(EXIT_FAILURE);
			}
			pthread_mutex_unlock(&data->philos[i].mut_last_eat_time);
		}

		usleep(1000); // Attendre un court instant avant de vérifier à nouveau
	}
	return (NULL);
}