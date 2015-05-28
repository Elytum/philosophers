/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xacoquan <xacoquan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/25 00:14:43 by xacoquan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <config.h>
# include <pthread.h>
# include <sys/types.h>

# define EAT_TIME EAT_T * 1000 * 1000
# define REST_TIME REST_T * 1000 * 1000
# define THINK_TIME THINK_T * 1000 * 1000
# define WAITING_TIME 5000
typedef enum	e_state
{
	EATING = 0,
	RESTING = 1,
	THINKING = 2
}				t_state;

typedef struct	s_phisolophe
{
	size_t		last_eat;
	size_t		last_sec;
	size_t		birth;
	size_t		id;
	ssize_t		life;
	t_state		state;
	size_t		turn;
	size_t		tmp;
	size_t		left;
	size_t		right;
}				t_phisolophe;

typedef struct	s_params
{
	size_t		id;
	size_t		birth;
}				t_params;

void			puterror(int output, char *str, size_t len, int out);
int				exit_function(int ret);
void			init_philosophers(t_params *params[PHILOSOPHERS_NB]);
int				wait_philosophers(void);
void			init_mutex(void);
int				philo(void *param);

int				g_requests[PHILOSOPHERS_NB + 1];
pthread_mutex_t g_chopsticks[PHILOSOPHERS_NB];
pthread_t		g_philosophers[PHILOSOPHERS_NB];

#endif
