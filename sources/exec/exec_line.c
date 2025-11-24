/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/24 21:01:44 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include "msh_utils.h"

// Returns 1 if a command was executed
static
int	stt_exec_simple(t_token *start, t_token *end, t_env *env)
{
	const int32_t	prev_fd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	t_vecp			argv;
	char			*arg_ptr[FT_ARG_COUNT];
	char			buf[FT_ARG_MAX];
	int				rvalue;

	if (prev_fd[0] < 0 || prev_fd[1] < 0)
		return (ft_error("msh_dup: ", NULL, 1));
	argv = (t_vecp){{buf, buf + sizeof(buf), buf}, 0, FT_ARG_COUNT, arg_ptr};
	if (msh_build_argv(start, env, &argv) <= 0 || !argv.ptr[0])
		return (1);
	msh_open_files(start, end, env);
	rvalue = msh_dispatch(&argv, env);
	if ((dup2(prev_fd[0], STDIN_FILENO) < 0)
		+ (dup2(prev_fd[1], STDOUT_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, 1);
	close(prev_fd[0]);
	close(prev_fd[1]);
	return (rvalue);
}

// Receives {start, end, env}
int	exec_stu(t_token *start, t_token *end, t_env *env)
{
	const int32_t	original_stdin = dup(STDIN_FILENO);
	t_token			*next;
	int				exit_status;

	if (original_stdin < 0)
		return (ft_error("msh_dup: ", NULL, 1));
	env->fd_tmp = original_stdin;
	if (start == end)
		return (env->exit_status);
	next = msh_next_delimiter(start, end, E_PIPE);
	if (next == end && !(start->type & E_OPAREN)
		&& msh_mutates_state(start, end))
		exit_status = stt_exec_simple(start, end, env);
	else
		exit_status = exec_pipeline(start, next, end, env);
	if ((dup2(original_stdin, STDIN_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);
	close(original_stdin);
	env->fd_tmp = -1;
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
