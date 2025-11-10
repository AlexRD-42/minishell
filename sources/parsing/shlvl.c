/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:51:14 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/07 18:51:31 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	\
stt_shlvl_valid(char *str, long *result)
{
	int		sign;
	long	value;

	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (!*str)
		return (0);
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		value = (value * 10) + *str++ - '0';
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str)
		return (0);
	*result = value * sign;
	return (1);
}

static char	*stt_shlvl_itoa(int level)
{
	char	new_level[6];
	long	temp;

	if (level < 10)
	{
		new_level[0] = level + '0';
		new_level[1] = '\0';
	}
	else if (level < 100)
	{
		new_level[0] = level / 10 + '0';
		new_level[1] = level % 10 + '0';
		new_level[2] = '\0';
	}
	else if (level < 1000)
	{
		new_level[0] = level / 100 + '0';
		temp = level % 100;
		new_level[1] = temp / 10 + '0';
		new_level[2] = temp % 10 + '0';
		new_level[3] = '\0';
	}
}

char	*shlvl(char *variable)
{
	const char	*shlvl_str = variable + 6;
	int			level;
	long		old_level;
	char		*new_level;

	if (!shlvl_str || !*shlvl_str || !stt_shlvl_valid(shlvl_str, &old_level))
		old_level = 0;
	level = old_level + 1;
	if (level < 0)
		level = 0;
	else if (level >= 1000)
	{
		write(2, "shell level too high, resetting to 1\n", 37);
		level = 1;
	}
	new_level = stt_shlvl_itoa(level);
	return (new_level);
}
