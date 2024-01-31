/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:30:02 by tsadouk           #+#    #+#             */
/*   Updated: 2024/01/31 15:07:46 by tsadouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	nb;

	i = 0;
	neg = 1;
	nb = 0;
	while (str[i] == '\n' || str[i] == '\t' || str[i] == '\r' || str[i] == '\f'
		|| str[i] == '\v' || str[i] == ' ')
		i++;
	if (str[i] == '-')
		neg = -neg;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (str[i] < 48 || str[i] > 57)
		return (0);
	while (str[i] >= 48 && str[i] <= 57)
	{
		nb = (nb * 10) + ((int)str[i] - 48);
		i++;
	}
	return (nb * neg);
}

void print_args(char **argv)
{
    printf("\033[0;33mnumbers of philosophers 😊: %d\033[0m\n", ft_atoi(argv[1])); // Marron pour "numbers of philosophers"
    printf("\033[1;31mtime to die ☠️⏰: %d\033[0m\n", ft_atoi(argv[2])); // Ajout de ☠️ à côté de "die" en rouge
    printf("\033[1;32mtime to eat 🍴 > %d\033[0m\n", ft_atoi(argv[3])); // Affichage de 🍴 suivi de ">" en vert
    printf("\033[1;34mtime to sleep 😴: %d\033[0m\n", ft_atoi(argv[4])); // Bleu clair pour "sleep"
    printf("\033[0;33mnumber of times each philosopher must eat 🍕: %d\033[0m\n", ft_atoi(argv[5])); // Marron pour "number of times each philosopher must eat"
}

bool	errors_handeler(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("\033[1;31mError: wrong number of arguments\033[0m\n");
		return (1);
	}
	return (0);
}


void	init_philosophers(t_data *data)
{
	int	i;

	if (!data->philos)
		return ;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	while (i < data->nb_philos)
	{
		data->philos[i].id = i;
		data->philos[i].meals_eaten = 0;
		data->philos[i].data = data;
		data->philos[i].state = IDLE;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
		pthread_mutex_init(&data->philos[i].mut_state, NULL);
		pthread_mutex_init(&data->philos[i].mut_meals_eaten, NULL);
		pthread_mutex_init(&data->philos[i].mut_last_eat_time, NULL);
		data->philos[i].last_eat_time = 0;
		i++;
	}
}

void init_all(t_data *data, char **argv)
{
	init_data(data, argv);
	init_mutex(data);
	init_forks(data);
	init_philosophers(data);
}


void create_philo_threads(t_data *data)
{
	int	i;

	if (!data->philo_ths)
		return ;

	i = 0;
	data->philo_ths = malloc(sizeof(pthread_t) * data->nb_philos);
	while (i < data->nb_philos)
	{
		if (pthread_create(&data->philo_ths[i], NULL, philo_routine, &data->philos[i]))
			exit(1);
		i++;
	}
}

int main(int argc, char **argv)
{
	if (errors_handeler(argc))
		return (1);
	print_args(argv);
	t_data data;
	init_all(&data, argv);
	create_philo_threads(&data);
	int i = 0;
	while (i < data.nb_philos)
	{
		pthread_join(data.philo_ths[i], NULL);
		i++;
	}
	free(data.philo_ths);
	free(data.philos);
	free(data.forks);

	return (0);
}