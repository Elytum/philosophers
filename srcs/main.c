#include <philo.h>

int					main(void)
{
	int				ret;
	t_params		*params[PHILOSOPHERS_NB];

	init_mutex();
	init_philosophers(params);
	ret = wait_philosophers();
	ret = exit_function(ret);
	return (ret);
}
