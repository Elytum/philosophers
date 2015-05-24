#include <philo.h>

int		main(void)
{
	pthread_mutex_t	chopsticks[PHILOSOPHERS_NB];
	pthread_t		philosophers[PHILOSOPHERS_NB];
	int				ret;
	t_params		*params[PHILOSOPHERS_NB];

	init_mutex(chopsticks);
	init_philosophers(philosophers, params);
	ret = wait_philosophers(philosophers);
	ret = exit_function(chopsticks, ret);
	while (42);
	return (ret);
}
