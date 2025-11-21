/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 15:45:06 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <linux/limits.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include "minishell.h"
#include "msh_utils.h"
#include "msh_defines.h"
#include "msh_types.h"

static const
char	*stt_find_path(char **envp)
{
	static const char	empty_path[2] = ":";
	char				*ptr;

	if (envp == NULL)
		return (empty_path);
	ptr = *envp;
	while (ptr != NULL)
	{
		if (ptr[0] == 'P' && ptr[1] == 'A' && ptr[2] == 'T' && ptr[3] == 'H'
			&& ptr[4] == '=')
			return (ptr + 5);
		ptr = *(++envp);
	}
	return (empty_path);
}

// Attemps to execute the command through the different paths offered
// CMD cannot exceed path size
static
int	stt_search_path(t_vecp *argv, t_env *env, size_t cmd_size)
{
	size_t		i;
	char		buffer[2 * FT_PATH_SIZE];
	char		*wptr;
	const char	*path = stt_find_path(env->ptr);	// Can use env_find 

	wptr = ft_memcpy(buffer + sizeof(buffer) - cmd_size, argv->ptr[0], cmd_size);
	*--wptr = '/';
	while (*path != 0)
	{
		i = 0;
		wptr[-1] = '.';
		while (path[i] != 0 && path[i] != ':')
			i++;
		if (i < FT_PATH_SIZE)
		{
			ft_memcpy(wptr - i, path, i);
			if (execve(wptr - i - (i == 0), argv->ptr, env->ptr) == -1)
				if (errno != ENOENT && errno != ENOTDIR)
					return (126);
		}
		path += i + (path[i] == ':');
	}
	return (127);
}

// To do: Check if expand_token null terminates the argument (it should)
ssize_t	msh_build_argv(t_token *token, t_env *env, t_vecp *argv)
{
	while ((token->type & E_CMD_END) == 0 && argv->count < argv->max_count - 1)
	{
		if (token->type & E_WORD)
		{
			if (expand_token(*token, env, argv) < 0)
				return (-1);
		}
		token++;
	}
	argv->ptr[argv->count] = NULL;
	return ((ssize_t) argv->count);
}

// Path only matters if its not a built-in or if it has absolute path!
int	exec_pipe(t_token *tokens, t_env *env)
{
	char		*arg_ptr[FT_ARG_COUNT];
	static char	buffer[FT_ARG_MAX];	
	t_vecp		argv;
	char		*cmd;
	size_t		length;

	argv = (t_vecp){{buffer, buffer + sizeof(buffer), buffer}, 0, FT_ARG_COUNT, arg_ptr};
	if (msh_build_argv(tokens, env, &argv) < 0)
		_exit(1);	// To do: Check if we still attempt a run on dir problems
	if (argv.ptr[0] == NULL || argv.count == 0)
		_exit(1); // FELIPE: 103 e 105 were "return (-1)" but were creating zombie child
	//msh_dispatch(&argv, env);	// Never returns on success
	cmd = argv.ptr[0];
	length = 0;
	while (cmd[length] != 0 && cmd[length] != '/')
		length++;
	if (cmd[length] == '/' && execve(cmd, argv.ptr, env->ptr))
		if (errno != ENOENT && errno != ENOTDIR)
			_exit(126);
	if (length < FT_PATH_SIZE && cmd[length] != '/')
		_exit(stt_search_path(&argv, env, length + 1));
	_exit(127);
}
