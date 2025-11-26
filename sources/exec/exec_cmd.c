/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 16:02:16 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	static const char	default_path[2] = ":";
	char				*ptr;

	if (envp == NULL)
		return (default_path);
	ptr = *envp;
	while (ptr != NULL)
	{
		if (ptr[0] == 'P' && ptr[1] == 'A' && ptr[2] == 'T' && ptr[3] == 'H'
			&& ptr[4] == '=')
			return (ptr + 5);
		ptr = *(++envp);
	}
	return (default_path);
}

// Attemps to execute the command through the different paths offered
// CMD cannot exceed path size
static
int	stt_search_path(t_vecp *argv, t_env *env, size_t cmd_size)
{
	size_t		i;
	char		buffer[2 * FT_PATH_SIZE];
	char		*wptr;
	const char	*path = stt_find_path(env->ptr);

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
	return (ft_error("msh_exec: command not found", "", 127));
}

// Path only matters if its not a built-in or if it has absolute path!
int	exec_cmd(t_token *tokens, t_env *env)
{
	t_vecp	argv;
	char	*cmd;
	char	*arg_ptr[FT_ARG_COUNT];
	char	buffer[FT_ARG_MAX];
	int		rvalue;

	argv = (t_vecp){{buffer, buffer + sizeof(buffer), buffer}, 0, FT_ARG_COUNT, arg_ptr};
	if (msh_build_argv(tokens, env, &argv) <= 0 || !argv.ptr[0])
		_exit(1);
	rvalue = msh_dispatch(&argv, env);
	if (rvalue != 127)
		_exit(rvalue);
	cmd = argv.ptr[0];
	while (*cmd != 0 && *cmd != '/')
		cmd++;
	if (*cmd == '/' && execve(argv.ptr[0], argv.ptr, env->ptr))
		if (errno != ENOENT && errno != ENOTDIR)
			_exit(126);
	if ((cmd - argv.ptr[0]) < FT_PATH_SIZE && *cmd != '/')
		_exit(stt_search_path(&argv, env, (size_t)(cmd - argv.ptr[0] + 1)));
	_exit(ft_error("msh_exec: command not found: ", argv.ptr[0], 127));
}
