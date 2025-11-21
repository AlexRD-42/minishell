/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:49 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 10:25:53 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_utils.h"

/* Echo prints out the arguments to stdout, with a space separating them
followed by a newline (-n suppresses the newline)
To do: Error handling */
static
int	stt_print_args(size_t count, uint8_t no_nl, char **envp)
{
	size_t		i;
	const char	*str;

	i = 1 + no_nl;
	while (i < count - 1)
	{
		str = envp[i];
		ft_write(STDOUT_FILENO, str, ft_strlen(str));
		ft_write(STDOUT_FILENO, " ", 1);
		i++;
	}
	ft_write(STDOUT_FILENO, str, ft_strlen(str));
	if (no_nl == 0)
		ft_write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	msh_echo(t_vecp *argv)
{
	uint8_t		no_nl;
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
	stt_print_args(argv->count, no_nl, argv->ptr);
	return (0);
}
