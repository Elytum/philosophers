/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xacoquan <xacoquan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/25 01:56:09 by xacoquan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int							g_nobody_died = 1;
extern pthread_mutex_t		g_chopsticks[PHILOSOPHERS_NB];

static inline t_phisolophe	create_philosopher(void *param)
{
	t_phisolophe			philo;
	t_params				*params;

	params = (t_params *)param;
	philo.newstate = 0;
	philo.id = params->id;
	philo.birth = params->birth;
	philo.state = RESTING;
	philo.life = MAX_LIFE;
	philo.last_eat = params->birth;
	philo.last_sec = params->birth;
	philo.turn = 0;
	return (philo);
}

static inline int			out_died(int ret)
{
	g_nobody_died = 0;
	return (ret);
}

static inline int			try_eating(t_phisolophe *philo)
{
	size_t					otherid;

	if (philo->id == 0)
		otherid = PHILOSOPHERS_NB - 1;
	else
		otherid = philo->id - 1;
	if (pthread_mutex_trylock(&(g_chopsticks[philo->id])))
		return (0);
	if (pthread_mutex_trylock(&(g_chopsticks[otherid])))
	{
		if (pthread_mutex_unlock(&(g_chopsticks[philo->id])))
		{
			g_nobody_died = 0;
			return (0);
		}
		return (0);
	}
	return (1);
}

static inline int			can_thing(t_phisolophe *philo)
{
	if (pthread_mutex_trylock(&(g_chopsticks[philo->id])))
		return (0);
	return (1);
}

/*
**	Gestion de l'update des philosophes
**		Gestion de la détection de la victoire
**		Gestion de la perte de vie
**		Tentative de prise de baguettes en cas de besoin
*/		

static inline int			update_philosopher(t_phisolophe *philo)
{
	static size_t			tmp;

	tmp = time(NULL);
	if (tmp > philo->birth + TIMEOUT)
		return (1);
	while (tmp > philo->last_sec)
	{
		philo->last_sec++;
		philo->life--;
		if (philo->life <= 0)
			return (out_died(0));
	}
	if ((philo->life <= MAX_LIFE * (3.5f / 4.f) ||
		(philo->turn == 0 && !(philo->id & 0b00000000000000000000000000000001)) ||
		(philo->turn == 1 &&  (philo->id & 0b00000000000000000000000000000001)))
					&& try_eating(philo))
	{
		philo->newstate = 1;
		philo->state = EATING;
	}
	else if (philo->life <= MAX_LIFE * (2.75f / 4.f) && can_thing(philo))
	{
		philo->newstate = 1;
		philo->state = THINKING;
	}
	else
	{
		philo->newstate = 1;
		philo->state = RESTING;
	}
	return (0);
}

/*
**	Gestion des états :
**		EATING		-> Attente puis gain de vie et activation du repos
**		RESTING		-> Perte de vie
**		THINKING	-> Perte de vie
*/

static inline void			process_eating(t_phisolophe *philo)
{
	size_t					otherid;

	philo->life = MAX_LIFE;
	usleep(EAT_T * 1000 * 1000);
	if (pthread_mutex_unlock(&(g_chopsticks[philo->id])))
	{
		g_nobody_died = 0;
		return ;
	}
	if (philo->id == 0)
		otherid = PHILOSOPHERS_NB - 1;
	else
		otherid = philo->id - 1;
	if (pthread_mutex_unlock(&(g_chopsticks[otherid])))
	{
		g_nobody_died = 0;
		return ;
	}
	philo->state = RESTING;
	philo->last_sec = time(NULL) + REST_T;
}

static inline int			update_state(t_phisolophe *philo)
{
	if (philo->state == EATING)
	{
		process_eating(philo);
		if (!g_nobody_died)
			return (1);
	}
	if (philo->state == RESTING)
		usleep(REST_T * 1000 * 1000);
	if (philo->state == THINKING)
	{
		usleep(THINK_T * 1000 * 1000);
		if (pthread_mutex_unlock(&(g_chopsticks[philo->id])))
		{
			g_nobody_died = 0;
			return (1);
		}
	}
	return (0);
}

int							philo(void *param)
{
	t_phisolophe			philo;

	philo = create_philosopher(param);
	while (philo.life > 0 && g_nobody_died)
	{
		if (update_philosopher(&philo))
			return (1);
		dprintf(1, "Philosopher %zi passing throug loop with %zi of life in state %zi\n", philo.id, philo.life, philo.state);
		if (update_state(&philo))
			break ;
		philo.turn++;
	}
	return (out_died(0));
}
