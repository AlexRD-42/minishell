/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:49 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 11:33:19 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

/* Echo prints out the arguments to stdout, with a space separating them
followed by a newline (-n suppresses the newline)
To do: Error handling */
int	msh_echo(int argc, const char **argv, t_env *env)
{
	size_t	i;
	uint8_t	no_nl;

	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (0);
	}
	no_nl = (argv[1][0] == '-') && (argv[1][1] == 'n') && (argv[1][2] == 0);
	if (argc == 2 && no_nl == 1)
		return (0);
	i = 1 + no_nl;
	while (i < (size_t) argc - 1)
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		write(STDOUT_FILENO, " ", 1);
		i++;
	}
	write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
	if (no_nl == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
