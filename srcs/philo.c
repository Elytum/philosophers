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
#include <signal.h>

extern int					g_requests[PHILOSOPHERS_NB + 1];
extern pthread_mutex_t		g_chopsticks[PHILOSOPHERS_NB];
extern pthread_t			g_philosophers[PHILOSOPHERS_NB];

static inline t_phisolophe	create_philosopher(void *param)
{
	t_phisolophe			philo;
	t_params				*params;

	params = (t_params *)param;
	philo.id = params->id;
	philo.birth = params->birth;
	philo.state = RESTING;
	philo.life = MAX_LIFE;
	philo.last_eat = params->birth;
	philo.last_sec = params->birth;
	philo.turn = 0;
	if (philo.id == 0)
		philo.left = PHILOSOPHERS_NB - 1;
	else
		philo.left = philo.id - 1;
	philo.right = philo.id;
	return (philo);
}

static inline int			out_died(int ret)
{
	g_requests[PHILOSOPHERS_NB] = 1;
	return (ret);
}

static inline int			can_eating(t_phisolophe *philo)
{
	if (pthread_mutex_trylock(&(g_chopsticks[philo->right])))
		return (0);
	if (pthread_mutex_trylock(&(g_chopsticks[philo->left])))
	{
		if (pthread_mutex_unlock(&(g_chopsticks[philo->right])))
		{
			g_requests[PHILOSOPHERS_NB] = 1;
			return (0);
		}
		return (0);
	}
	return (1);
}

static inline int			can_think(t_phisolophe *philo)
{
	if (pthread_mutex_trylock(&(g_chopsticks[philo->right])))
		return (0);
	return (1);
}

/*
**	Gestion de l'update des philosophes
**		Gestion de la détection de la victoire
**		Gestion de la perte de vie
**		Tentative de prise de baguettes en cas de besoin
*/		

int							starving(t_phisolophe *philo)
{
	if (philo->life < REST_T && philo->life < THINK_T)
	{
		dprintf(1, "Philosopher %zi starving\n", philo->id);
		if (philo->state != THINKING &&
				pthread_mutex_trylock(&(g_chopsticks[philo->right])))
		{
			dprintf(1, "%zi has an already used chopstick\n", philo->right);
			return (0);
		}
		if (pthread_mutex_trylock(&(g_chopsticks[philo->left])))
		{
			dprintf(1, "%zi asking assistance from philosopher %zi\n", philo->right, philo->left);
			g_requests[philo->left] = 1;
			usleep((WAITING_TIME + 1000) * 10);
			g_requests[philo->left] = 0;
			if (pthread_mutex_trylock(&(g_chopsticks[philo->left])))
			{
				dprintf(1, "%zi can't access to left chopstick\n", philo->right);
				pthread_mutex_unlock(&(g_chopsticks[philo->right]));
				return (0);
			}
		}
		philo->state = EATING;
		return (1);
	}
	return (0);
}

int							life_loss(t_phisolophe *philo)
{
	while ((size_t)time(NULL) > philo->last_sec)
	{
		philo->last_sec++;
		philo->life--;
		if (philo->life <= 0)
		{
			g_requests[PHILOSOPHERS_NB] = 1;
			return (0);
		}
	}
	return (1);
}

int							should_eat(t_phisolophe *philo)
{
	usleep(10000);
	if (!(g_requests[philo->right]))
		return (1);
	return ((philo->life <= MAX_LIFE * (3.5f / 4.f) ||
		(philo->turn == 0 &&
					!(philo->id & 0b00000000000000000000000000000001)) ||
		(philo->turn == 1 &&
					(philo->id & 0b00000000000000000000000000000001))));
}

int							should_think(t_phisolophe *philo)
{
	return ((philo->life < THINK_T * 2 &&
			philo->id & 0b00000000000000000000000000000001) ||
		philo->life <= MAX_LIFE * (2.5f / 4.f));
}


static inline int			update_philosopher(t_phisolophe *philo)
{
	if ((size_t)time(NULL) > philo->birth + TIMEOUT)
		return (1);
	if (!life_loss(philo) || starving(philo))
		return (0);
	if (should_eat(philo) && can_eating(philo))
		philo->state = EATING;
	else if (should_think(philo) && can_think(philo))
		philo->state = THINKING;
	else
		philo->state = RESTING;
	return (0);
}

/*
**	Gestion des états :
**		EATING		-> Attente puis gain de vie et activation du repos
**		RESTING		-> Perte de vie
**		THINKING	-> Perte de vie
*/

static inline void			do_eat(t_phisolophe *philo)
{
	philo->tmp = time(NULL);
	while ((size_t)time(NULL) < philo->tmp + EAT_T)
		usleep(EAT_TIME);
	philo->life = MAX_LIFE;
	pthread_mutex_unlock(&(g_chopsticks[philo->left]));
	pthread_mutex_unlock(&(g_chopsticks[philo->right]));
	philo->last_sec = time(NULL) + REST_T;
	usleep(REST_TIME);
}

void						do_think(t_phisolophe *philo)
{
	philo->tmp = time(NULL);
	while ((size_t)time(NULL) < philo->tmp + THINK_T)
	{
		usleep(WAITING_TIME);
		if (g_requests[philo->right])
		{
			dprintf(1, "%zi stopped thinking in order to save philosopher %zi\n", philo->right, (philo->right + 1) % PHILOSOPHERS_NB);
			pthread_mutex_unlock(&(g_chopsticks[philo->right]));
			while ((size_t)time(NULL) < philo->tmp + THINK_T)
				usleep(WAITING_TIME);
			return ;
		}
	}
	pthread_mutex_unlock(&(g_chopsticks[philo->right]));
}

void						do_sleep(void)
{
	usleep(REST_TIME);
}

static inline int			update_state(t_phisolophe *philo)
{
	if (philo->state == EATING)
		do_eat(philo);
	if (philo->state == RESTING)
		do_sleep();
	if (philo->state == THINKING)
		do_think(philo);
	return (0);
}

int							philo(void *param)
{
	t_phisolophe			philo;

	philo = create_philosopher(param);
	while (philo.life > 0 && !g_requests[PHILOSOPHERS_NB])
	{
		if (update_philosopher(&philo))
			return (1);
		dprintf(1, "Philosopher %zi acting with %zi of life in state %zi\n", philo.id, philo.life, philo.state);
		if (update_state(&philo))
			break ;
		philo.turn++;
	}
	return (out_died(0));
}
