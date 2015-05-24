/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achazal <achazal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/03 04:07:04 by achazal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int	g_nobody_died = 1;

t_phisolophe		create_philosopher(void *param)
{
	t_phisolophe	philo;
	t_params		*params;
	static int		inc = 0;

	params = (t_params *)param;
	dprintf(1, "Giving birth to : %zi\n", params->id);
	philo.id = params->id;
	philo.birth = params->birth;
	philo.state = RESTING;
	philo.life = MAX_LIFE + inc;
	inc += 5;
	philo.last_eat = params->birth;
	philo.last_sec = params->birth;
	return (philo);
}

int					update_philosopher(t_phisolophe *philo)
{
	static size_t	tmp;

	tmp = time(NULL);
	if (tmp > philo->birth + TIMEOUT)
		return (1);
	if (tmp > philo->last_sec)
	{
		philo->last_sec = tmp;
		philo->life--;
	}
	return (0);
}

int					philo(void *param)
{
	t_phisolophe	philo;

	philo = create_philosopher(param);
	while (philo.life > 0 && g_nobody_died)
	{
		if (update_philosopher(&philo))
			return (1);
	}
	if (g_nobody_died == 1)
		g_nobody_died = 0;
	dprintf(1, "Philosophe %zi died\n", philo.id);
	return (0);
}
