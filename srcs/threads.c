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

void		init_philosophers(pthread_t philosophers[PHILOSOPHERS_NB],
									t_params *params[PHILOSOPHERS_NB])
{
	int		id;
	size_t	birth;

	id = 0;
	birth = time(NULL);
	while (id < PHILOSOPHERS_NB)
	{
		if (!(params[id] = (t_params *)malloc(sizeof(t_params))))
			 puterror(2, "Malloc error\n", 13, 2);
		params[id]->id = id;
		params[id]->birth = birth;
		pthread_create(&(philosophers[id]), NULL,
						(void *)philo, (void *)((params[id])));
		++id;
	}
}

int			wait_philosophers(pthread_t philosophers[PHILOSOPHERS_NB])
{
	int		id;
	int		ret;

	ret = 0;
	id = 0;
	while (id < PHILOSOPHERS_NB)
	{
		if (ret == 0)
			pthread_join(philosophers[id], (void **)(&ret));
		else
			pthread_join(philosophers[id], NULL);
		id++;
	}
	return (ret);
}
