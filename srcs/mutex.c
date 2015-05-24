/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achazal <achazal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/03 04:07:04 by achazal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void		init_mutex(pthread_mutex_t chopsticks[PHILOSOPHERS_NB])
{
	int		id;

	id = 0;
	while (id < PHILOSOPHERS_NB)
	{
		if (pthread_mutex_init(&(chopsticks[id++]), NULL))
			puterror(2, "Could not initialize mutexes\n", 29, 2);
	}
}
