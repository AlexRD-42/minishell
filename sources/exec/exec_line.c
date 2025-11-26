/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/26 17:58:38 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include <unistd.h>

static
int	stt_exec_simple(t_token *start, t_token *end, t_env *env)
{
	t_vecp	argv;
	char	*arg_ptr[FT_ARG_COUNT];
	char	buf[FT_ARG_MAX];
	int		fd[2];
	int		rvalue;

	argv = (t_vecp){{buf, buf + sizeof(buf), buf}, 0, FT_ARG_COUNT, arg_ptr};
	fd[0] = -1;
	fd[1] = -1;
	if (msh_build_argv(start, end, env, &argv) <= 0 || !argv.ptr[0])		// Fatal Error
		return (1);
	if (msh_open_files(start, end, env, fd) == -1)
		return (1);
	rvalue = msh_dispatch(&argv, env);
	if (fd[0] >= 0)
		close(fd[0]);
	if (fd[1] >= 0)
		close(fd[1]);
	return (rvalue);
}

// Receives {start, end, env}
int	exec_stu(t_token *start, t_token *end, t_env *env)
{
	t_token	*next;
	int		exit_status;

	if (start == end)
		return (env->exit_status);
	next = msh_next_delimiter(start, end, E_PIPE);
	if (next == end && !(start->type & E_OPAREN) && msh_mutates_state(start, end))
		exit_status = stt_exec_simple(start, end, env);
	else
		exit_status = exec_pipeline(start, next, end, env);
	return (exit_status);
}

int	exec_line(t_token *start, t_token *end, t_env *env)
{
	t_token	*next;
	t_token	*current;
	bool	should_run;

	if (start == end)
		return (env->exit_status);
	current = start;
	should_run = 1;
	while (current < end)
	{
		next = msh_next_delimiter(current, end, E_AND | E_OR);
		if (should_run)
			env->exit_status = exec_stu(current, next, env);
		if (next == end)
			break ;
		if ((next->type & E_AND))
			should_run = env->exit_status == 0;
		else if ((next->type & E_OR))
			should_run = env->exit_status != 0;
		current = next + 1;
	}
	return (env->exit_status);
}
