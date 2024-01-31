/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 14:58:00 by tsadouk           #+#    #+#             */
/*   Updated: 2024/01/31 15:00:22 by tsadouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **argv)
{
	data->nb_philos = ft_atoi(argv[1]);
	data->nb_meals = ft_atoi(argv[5]);
	data->nb_full_p = 0;
	data->keep_iterating = true;
	data->eat_time = ft_atoi(argv[3]);
	data->die_time = ft_atoi(argv[2]);
	data->sleep_time = ft_atoi(argv[4]);
	data->start_time = 0;
}

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->mut_eat_t, NULL);
	pthread_mutex_init(&data->mut_die_t, NULL);
	pthread_mutex_init(&data->mut_sleep_t, NULL);
	pthread_mutex_init(&data->mut_print, NULL);
	pthread_mutex_init(&data->mut_nb_philos, NULL);
	pthread_mutex_init(&data->mut_keep_iter, NULL);
	pthread_mutex_init(&data->mut_start_time, NULL);
}

void	init_forks(t_data *data)
{
	int	i;

	if (!data->forks)
		return ;
	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	while (i < data->nb_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
}
