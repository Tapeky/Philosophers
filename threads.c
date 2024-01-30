/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsadouk <tsadouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 13:55:13 by tsadouk           #+#    #+#             */
/*   Updated: 2024/01/29 14:33:51 by tsadouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void	exit_on_error(int *num, int exit_code)
{
	free(num);
	exit(exit_code);
}

void	*routine_2(void *arg)
{
	int *num;
	int i;

	num = (int *)arg;
	i = 0;
	while (i < 100)
	{
		(*num)++;
		i++;
	}
	return (NULL);
}

void	*print_hello(void *arg)
{
	int	i;

	i = 0;
	while (i < 100)
	{
		printf("\033[1;31mHELLO\n\033[0m");
		i++;
		usleep(100);
	}
	return (NULL);
}

void	*print_world(void *arg)
{
    int i;

    i = 0;
    while (i < 100)
    {
        printf("\033[1;34mWORLD\n\033[0m");
        i++;
        usleep(100);
    }

    return NULL;
}

void	exemple_1()
{
	print_hello(NULL);
	print_world(NULL);
}

void	exemple_2()
{
	pthread_t	thread_1;
	pthread_t	thread_2;

	if (pthread_create(&thread_1, NULL, routine_2, NULL))
		exit(1);
	if (pthread_create(&thread_2, NULL, routine_2, NULL))
		exit(1);
	if (pthread_join(thread_1, NULL))
		exit(1);
	if (pthread_join(thread_2, NULL))
		exit(1);
}

void	exemple_3()
{
	int 	*num;
	pthread_t	thread_1;
	pthread_t	thread_2;

	num = malloc(sizeof(int));
	if (num == NULL)
		exit(1);
	*num = 0;
	if (pthread_create(&thread_1, NULL, routine_2, num))
		exit_on_error(num, 2);
	if (pthread_create(&thread_2, NULL, routine_2, num))
		exit_on_error(num, 2);
	if (pthread_join(thread_1, NULL))
		exit_on_error(num, 3);
	if (pthread_join(thread_2, NULL))
		exit_on_error(num, 3);
	printf("num = %d\n", *num);

}

int main(void)
{
	exemple_3();

	return 0;
}