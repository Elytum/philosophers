/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achazal <achazal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/03 04:07:04 by achazal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

#include <stdio.h>
#include <stdlib.h>

void					init_philosophers(t_params *params[PHILOSOPHERS_NB])
{
	extern pthread_t	g_philosophers[PHILOSOPHERS_NB];
	int					id;
	size_t				birth;

	id = 0;
	birth = time(NULL);
	while (id < PHILOSOPHERS_NB)
	{
		if (!(params[id] = (t_params *)malloc(sizeof(t_params))))
			 puterror(2, "Malloc error\n", 13, 2);
		params[id]->id = id;
		params[id]->birth = birth;
		pthread_create(&(g_philosophers[id]), NULL,
						(void *)philo, (void *)((params[id])));
		++id;
	}
}

void					init_requests(void)
{
	extern int			g_requests[PHILOSOPHERS_NB + 1];
	size_t				i;

	i = 0;
	while (i <= PHILOSOPHERS_NB)
	{
		g_requests[i] = 0;
		i++;
	}
}

int						wait_philosophers(void)
{
	extern pthread_t	g_philosophers[PHILOSOPHERS_NB];
	int					id;
	int					ret;

	ret = 0;
	id = 0;
	dprintf(1, "Waiting philosophers");
	while (id < PHILOSOPHERS_NB)
	{
		if (ret == 0)
			pthread_join(g_philosophers[id], (void **)(&ret));
		else
			pthread_join(g_philosophers[id], NULL);
		id++;
	}
	return (ret);
}
