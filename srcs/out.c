/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achazal <achazal@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/03 04:07:02 by achazal           #+#    #+#             */
/*   Updated: 2015/05/03 04:07:04 by achazal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>
#include <stdlib.h>
#include <unistd.h>

void	puterror(int output, char *str, size_t len, int out)
{
	write(output, str, len);
	exit(out);
}
#include <stdio.h>

int		exit_function(pthread_mutex_t chopsticks[PHILOSOPHERS_NB], int ret)
{
	char	c;
	int		id;

	id = 0;
	while (id < PHILOSOPHERS_NB)
		pthread_mutex_destroy(&(chopsticks[id++]));
	if (ret == 1)
		write(1, "Now, it is time... To DAAAAAAAANCE ! ! !", 40);
	else
		write(1, "HAHA, bad chance ! They died, and you're all alone here. "
			"I was right, you make a very lame subject. "
			"Hope that one named shell will do better.", 141);
	read(1, &c, 1);
	return (0);
}
