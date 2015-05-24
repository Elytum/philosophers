/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xacoquan <xacoquan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/24 23:37:38 by xacoquan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void						init_mutex(void)
{
	extern pthread_mutex_t	g_chopsticks[PHILOSOPHERS_NB];
	int						id;

	id = 0;
	while (id < PHILOSOPHERS_NB)
	{
		if (pthread_mutex_init(&(g_chopsticks[id++]), NULL))
			puterror(2, "Could not initialize mutexes\n", 29, 2);
	}
}

