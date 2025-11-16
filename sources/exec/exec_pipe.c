/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/15 22:55:41 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_defines.h"
#include <linux/limits.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_types.h"

// Attemps to execute the command through the different paths offered
// If it returns, then it failed to execute the command
// To do: change to exit instead of return
static
int	search_path(t_argv *argv, t_env *env, const char *buffer, )
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

// To do: Check if expand_token null terminates the argument (it should)
// To do: The first argument (command) cannot exceed PATH_SIZE
int	msh_build_argv(t_token *token, t_env *env, t_argv *arg)
{
	int		rvalue;

	rvalue = 0;
	while ((token->type & E_CMD_END) == 0)
	{
		if (token->type & E_WORD)
			rvalue = expand_token(token, env, arg, FT_ARG_COUNT);
		if (rvalue != 0)
			break ;
		token++;
	}
	arg->ptr[arg->count] = NULL;
	return (rvalue);
}

// Copies the command to the end of the buffer, so that when trying all paths
static
int	parse_cmd(t_argv *argv, char *path, t_env *env)
{
	char	*cmd;
	size_t	length;

	if (argv->ptr[0] == NULL || argv->count == 0)
		return (1);
	cmd = argv->ptr[0];
	while (*cmd == ' ' || (*cmd >= 9 && *cmd <= 13))
		cmd++;
	argv->ptr[0] = cmd;
	length = 0;
	while (cmd[length] != 0 && cmd[length] != '/')
		length++;
	if (cmd[length++] == '/')
		_exit(execve(cmd, argv->ptr, env->ptr));	// execve runs with absolute path, no path building occurs
	// msh_dispatch(); if it returns, its not a built-in!
	ft_memcpy(path + FT_PATH_SIZE - length, cmd, length);
}

// Path only matters if its not a built-in or if it has absolute path!
int	pipe_exec(t_token *tokens, t_env *env)
{
	char	*arg_ptr[FT_ARG_COUNT];
	char	buffer[FT_ARG_SIZE];
	char	path[FT_PATH_SIZE];
	int		rvalue;
	t_argv	argv;

	argv = (t_argv){0, 0, buffer, arg_ptr, buffer + sizeof(buffer)};
	rvalue = msh_build_argv(tokens, env, &argv);
	
	ft_memcpy()
	return ();
}
