#include <philo.h>

int					main(void)
{
	pthread_t		philosophers[PHILOSOPHERS_NB];
	int				ret;
	t_params		*params[PHILOSOPHERS_NB];

	init_mutex();
	init_philosophers(philosophers, params);
	ret = wait_philosophers(philosophers);
	ret = exit_function(ret);
	// while (42);
	return (ret);
}
