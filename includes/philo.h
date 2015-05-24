/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xacoquan <xacoquan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/24 23:36:19 by xacoquan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <config.h>
# include <pthread.h>
# include <sys/types.h>

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
}				t_phisolophe;

typedef struct	s_params
{
	size_t		id;
	size_t		birth;
}				t_params;

void			puterror(int output, char *str, size_t len, int out);
int				exit_function(int ret);
void			init_philosophers(pthread_t philosophers[PHILOSOPHERS_NB],
										t_params *params[PHILOSOPHERS_NB]);
int				wait_philosophers(pthread_t philosophers[PHILOSOPHERS_NB]);
void			init_mutex(void);
int				philo(void *param);


pthread_mutex_t g_chopsticks[PHILOSOPHERS_NB];

#endif
