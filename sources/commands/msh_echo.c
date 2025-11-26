/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:49 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 09:23:40 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_utils.h"

/* Echo prints out the arguments to stdout, with a space separating them
followed by a newline (-n suppresses the newline) */
static
int	stt_print_args(bool no_nl, char **argv)
{
	if (argv[0] != NULL)
	{
		while (argv[1] != NULL)
		{
			if (ft_write(STDOUT_FILENO, argv[0], ft_strlen(argv[0])) == -1)
				return (1);
			if (ft_write(STDOUT_FILENO, " ", 1) == -1)
				return (1);
			argv++;
		}
		if (ft_write(STDOUT_FILENO, argv[0], ft_strlen(argv[0])) == -1)
			return (1);
	}
	if (no_nl == 1)
		if (ft_write(STDOUT_FILENO, "\033[7m%\033[27m", 10) == -1)
			return (1);
	if (ft_write(STDOUT_FILENO, "\n", 1) == -1)
		return (1);
	return (0);
}

int	msh_echo(t_vecp *argv)
{
	bool		no_nl;
	const char	*str;

	if (argv->count < 2)
	{
		ft_write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	str = argv->ptr[1];
	no_nl = (str[0] == '-') && (str[1] == 'n') && (str[2] == 0);
	if (argv->count == 2 && no_nl == 1)
		return (0);
	return (stt_print_args(no_nl, argv->ptr + no_nl + 1));
}
