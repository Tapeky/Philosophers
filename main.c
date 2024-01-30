/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 14:30:02 by tsadouk           #+#    #+#             */
/*   Updated: 2024/01/30 16:20:18 by tsadouk          ###   ########.fr       */
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

void initialize_philos(t_philo *philo, char **argv, int nb_philo)
{
	int i;

	i = 0;
	while (i < nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].time_to_die = ft_atoi(argv[2]);
		philo[i].time_to_eat = ft_atoi(argv[3]);
		philo[i].time_to_sleep = ft_atoi(argv[4]);
		philo[i].nb_eat = ft_atoi(argv[5]);
		philo[i].nb_philo = nb_philo;
		philo[i].nb_fork = nb_philo;
		philo[i].meals_eaten = 0;
		i++;
	}
}


bool	errors_handeler(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		printf("\033[1;31mError: wrong number of arguments\033[0m\n");
		return (1);
	}
	return (0);
}

void	*philoshopher_thread(void *arg)
{
	t_philo *philosopher = (t_philo *)arg;

	while (1)
	{
		// Is thinking
		printf("\033[1;34mPhilosopher %d is thinking\n\033[0m", philosopher->id);
		sleep(philosopher->time_to_sleep);

		// Take a fork
		pthread_mutex_lock(&philosopher->mutex[philosopher->id - 1]);
		printf("\033[1;33mPhilosopher %d has taken a fork\n\033[0m", philosopher->id);

		// Tale the second fork
		pthread_mutex_lock(&philosopher->mutex[philosopher->id % philosopher->nb_philo]);
		printf("\033[1;33mPhilosopher %d has taken a fork\n\033[0m", philosopher->id);

		// Is eating
		printf("\033[1;32mPhilosopher %d is eating\n\033[0m", philosopher->id);
		philosopher->meals_eaten++;
		sleep(philosopher->time_to_eat);

		// Put down the forks
		pthread_mutex_unlock(&philosopher->mutex[philosopher->id - 1]);
		pthread_mutex_unlock(&philosopher->mutex[philosopher->id % philosopher->nb_philo]);

		// The philosopher has eaten enough ?
		if (philosopher->meals_eaten == philosopher->nb_eat)
			break;
		
		// Is sleeping
		printf("\033[1;34mPhilosopher %d is sleeping\n\033[0m", philosopher->id);
		sleep(philosopher->time_to_sleep);

	}

}

void	create_philos_trhead(t_philo *philo, int nb_philo)
{
	int i;

	i = 0;
	while (i < nb_philo)
	{
		pthread_create(&philo[i].thread, NULL, philoshopher_thread, &philo[i]);
		i++;
	}
}


void	free_all_philos(t_philo *philosophers, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		free(&philosophers[i]);
		i++;
	}
	free(philosophers);
}

int	main(int argc, char **argv)
{
	int		nb_philosophers;
	t_philo	*philosophers;

	if (errors_handeler(argc, argv))
		return (0);
	nb_philosophers = ft_atoi(argv[1]);
	philosophers = (t_philo *)malloc(sizeof(t_philo) * nb_philosophers);
	if (!philosophers)
		return (0);

	print_args(argv);
	initialize_philos(philosophers, argv, nb_philosophers);
	create_philos_trhead(philosophers, nb_philosophers);

	for (int i = 0; i < nb_philosophers; i++)
		pthread_join(philosophers[i].thread, NULL);

	free_all_philos(philosophers, nb_philosophers);
	return (0);
}