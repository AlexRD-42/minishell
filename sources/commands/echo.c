/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:25:13 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/23 15:41:36 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Echo prints out the arguments to the standard output, with a space separating
them followed by a newline (-n suppresses the newline)
*/

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <minishell.h>

// To do: Error handling
int	msh_echo(int argc, const char **argv)
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
