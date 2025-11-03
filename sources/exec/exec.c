/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:23:48 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/03 15:11:52 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "minishell.h"

static
int	exec_error(int error_code)
{
	if (error_code == 1)
		write(2, "Pipex Error: Empty Path\n", 24);
	else if (error_code == 2)
		write(2, "Pipex Error: Path too long\n", 27);
	else
		perror("execve");
	return (-1);
}

// Could also reserve space for path
static const
char	*find_path(const char **envp)
{
	const char	*ptr;

	if (envp == NULL)
		return (NULL);
	ptr = *envp;
	while (ptr != NULL)
	{
		if (ptr[0] == 'P' && ptr[1] == 'A' && ptr[2] == 'T' && ptr[3] == 'H'
			&& ptr[4] == '=')
			return (ptr + 5);
		ptr = *(envp++);
	}
	return (NULL);
}

static
size_t	in_place_split(char *cmd, char **buffer)
{
	size_t	i;
	size_t	break_flag;

	i = 0;
	while (*cmd != 0 && i < (FT_ARG_MAX - 1))
	{
		while (*cmd == ' ')
			cmd++;
		buffer[i++] = cmd;
		break_flag = 0;
		while (*cmd != 0)
		{
			break_flag += *cmd == '"';
			if (*cmd == ' ' && !(break_flag & 1))
				break ;
			cmd++;
		}
		if (*cmd == ' ')
			*cmd++ = 0;
	}
	buffer[i] = NULL;
	i = 0;
	while (buffer[0][i] != 0)
		i++;
	return (i);
}

// Attemps to execute the command through the different paths offered
// If it returns, then it failed to execute the command
static
int	exec_cmd(char *cmd, char **argv, char **envp, size_t cmd_length)
{
	size_t			i;
	char			buffer[FT_PATH_MAX];
	const char		*path = find_path(envp);
	int				error_code;

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

int	pipe_exec(char *cmd, char **envp)
{
	char	*argv[FT_ARG_MAX];
	size_t	i;
	size_t	cmd_length;
	int		error_code;

	i = 0;
	if (*cmd == 0)
	{
		argv[0] = cmd;
		argv[1] = NULL;
		return (exec_error(execve(cmd, argv, envp)));
	}
	cmd_length = in_place_split(cmd, argv) + 1;
	while (cmd[i] != 0 && cmd[i] != '/')
		i++;
	if (cmd[i] == '/')
		error_code = execve(cmd, argv, envp);
	else
		error_code = exec_cmd(cmd, argv, envp, cmd_length);
	return (exec_error(error_code));
}
