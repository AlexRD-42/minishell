/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:49 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 22:02:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_utils.h"

static
int	stt_parse_str(const char *str, int64_t *number)
{
	const char	*ostr = str;

	while (*ostr == ' ' || (*ostr >= '\t' && *ostr <= '\r'))
		ostr++;
	str = ostr + (*str == '-' || *str == '+');
	while (*str != 0)
	{
		if (*str < '0' || *str > '9')
			return (-1);
		str++;
	}
	if (str - ostr > 11)
		return (-1);
	*number = ft_atoi(ostr);
	if (*number > INT_MAX || *number < INT_MIN)
		return (-1);
	return (0);
}

// prints exit always
// Returns 1 on too many arguments, 2 on numeric
int	msh_exit(t_vecp *argv, t_env *env)
{
	int64_t	number;

	(void) env;
	number = 0;
	if (argv->count > 2)
		return (ft_error("msh_exit: too many arguments", "", 1));
	if (argv->count == 1)
		_exit(env->exit_status & 0xFF);
	if (stt_parse_str(argv->ptr[1], &number))
		return (ft_error("msh_exit: numeric argument required", "", 2));
	_exit(number & 0xFF);	// Review: _exit or exit
}
