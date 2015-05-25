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

int						g_nobody_died = 1;
extern pthread_mutex_t	g_chopsticks[PHILOSOPHERS_NB];

t_phisolophe		create_philosopher(void *param)
{
	t_phisolophe	philo;
	t_params		*params;

	params = (t_params *)param;
	// dprintf(1, "Giving birth to : %zi\n", params->id);
	philo.newstate = 0;
	philo.id = params->id;
	philo.birth = params->birth;
	philo.state = RESTING;
	philo.life = MAX_LIFE;
	philo.last_eat = params->birth;
	philo.last_sec = params->birth;
	return (philo);
}

int					out_died(int ret)
{
	g_nobody_died = 0;
	return (ret);
}


int					try_eating(t_phisolophe *philo)
{
	size_t			otherid;

	if (philo->id == 0)
		otherid = PHILOSOPHERS_NB - 1;
	else
		otherid = philo->id - 1;
	// dprintf(1, "Process %zi trying to eating with %zi and %zi\n", philo->id, philo->id, otherid);
	if (pthread_mutex_trylock(&(g_chopsticks[philo->id])))
	{
		return (0);
	}
	// dprintf(1, "Process %zi can't eat because %zi is used at %zi\n", philo->id, philo->id, time(NULL));
	if (pthread_mutex_trylock(&(g_chopsticks[otherid])))
	{
		if (pthread_mutex_unlock(&(g_chopsticks[philo->id])))
		{
			g_nobody_died = 0;
			return (0);
		}
		// dprintf(1, "Process %zi can't eat because %zi is used at %zi\n", philo->id, otherid, time(NULL));
		return (0);
	}
	// dprintf(1, "Process %zi will eat with %zi and %zi at %zi\n", philo->id, philo->id, otherid, time(NULL));
	// dprintf(1, "Process %zi will eat with %zi (%p) and %zi (%p) at %zi\n", philo->id, philo->id, &g_chopsticks[philo->id], otherid, &g_chopsticks[otherid], time(NULL));
	return (1);
}

int					can_thing(t_phisolophe *philo)
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

int					update_philosopher(t_phisolophe *philo)
{
	static size_t	tmp;

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
	if (philo->life <= MAX_LIFE * (3.5f / 4.f) && try_eating(philo))
	{
		philo->newstate = 1;
		philo->state = EATING;
	}
	else if (philo->life <= MAX_LIFE * (2.5f / 4.f) && can_thing(philo))
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
**		RESTING		-> Perte de vie, détection de la défaite puis attente
**		THINKING	-> Perte de vie, détection de la défaite puis attente
*/

static inline void	process_eating(t_phisolophe *philo)
{
	size_t			otherid;

	// dprintf(1, "Philosopher %zi sleeping for %zi seconds (time = %zi), life = %zi\n", philo->id, EAT_T, time(NULL), philo->life);
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
	// dprintf(1, "Philosopher %zi woke up (time = %zi), life = %zi\n", philo->id, time(NULL), philo->life);
}

int					update_state(t_phisolophe *philo)
{
	// if (philo->newstate == 1)
	// {
		// dprintf(1, "Philosophe %zu is being updated, life = %zu, state = %i\n", philo->id, philo->life, philo->state);
		// philo->newstate = 0;
		if (philo->state == EATING)
		{
			process_eating(philo);
			if (!g_nobody_died)
				return (1);
		}
		if (philo->state == RESTING)
		{
			// if (philo->life <= REST_T)
			// 	return (out_died(1));
			// philo->life -= REST_T;
			// if (!g_nobody_died)
			// 	return (1);
			usleep(REST_T * 1000 * 1000);
			// if (!g_nobody_died)
				// return (1);
		}
		if (philo->state == THINKING)
		{
			// if (philo->life <= THINK_T)
			// 	return (out_died(1));
			// philo->life -= THINK_T;
			// if (!g_nobody_died)
			// 	return (1);
			usleep(THINK_T * 1000 * 1000);
			// if (!g_nobody_died)
				// return (1);
			if (pthread_mutex_unlock(&(g_chopsticks[philo->id])))
			{
				g_nobody_died = 0;
				return (1);
			}
		}
		// dprintf(1, "Philosophe %zu have been updated, life = %zu\n", philo->id, philo->life);
	// }
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
		dprintf(1, "Philosopher %zi passing throug loop with %zi of life in state %zi\n", philo.id, philo.life, philo.state);
		if (update_state(&philo))
			break ;
	}
	// dprintf(1, "Philosophe %zi died\n", philo.id);
	return (out_died(0));
}
