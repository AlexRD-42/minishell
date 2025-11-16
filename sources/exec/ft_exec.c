/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 18:11:08 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 19:12:36 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "minishell.h"

static
int	msh_execve(const char *path, t_argv *argv, t_env *env)
{
	size_t	i;
	int		error_code;

	error_code = 1;
	while (*path != 0)
	{
		i = 0;
		while (path[i] != 0 && path[i] != ':')
			i++;
		if (i + cmd_length >= 4094)
			return (2);
		if (i == 0)
			buffer[i++] = '.';
		else
			ft_memcpy(buffer, path, i);
		path += i + (path[i] == ':');
		buffer[i++] = '/';
		ft_memcpy(buffer + i, cmd, cmd_length);
		error_code = execve(buffer, argv, envp);
	}
	return (error_code);
}