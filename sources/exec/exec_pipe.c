/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 20:50:06 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 18:03:37 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include "msh_utils.h"
#include <stdlib.h>
#include <signal.h>

// Always exits upon return ((echo))
static
int	stt_exec_subshell(t_token *start, t_env *env)
{
	t_token		*end;
	size_t		pdepth;

	end = ++start;
	pdepth = 1 - !!(end->type & E_CPAREN);	// Never run the loop if its empty
	while (pdepth != 0)	// Dangerous if pre-validation is wrong but good for debugging
	{
		pdepth += !!(end->type & E_OPAREN) - !!(end->type & E_CPAREN);
		end++;
	}
	_exit (exec_line(start, end, env));
}

static
pid_t	stt_child(int32_t *pipe_fd, t_token *current, t_token *next, t_env *env)
{
	const bool	not_subshell = !(current->type & E_OPAREN);

	sigaction(SIGINT, env->sig_dfl, NULL);
	sigaction(SIGQUIT, env->sig_dfl, NULL);
	sigaction(SIGWINCH, env->sig_dfl, NULL);
	if (msh_open_files(current, next, env, pipe_fd) == -1)	// Pipe fd gets overriden and closed
		return (1);
	if (ft_dup2(pipe_fd[1], STDOUT_FILENO) == -1)	// ft_dup already closes fd[1] and prints
		return (1);
	if (pipe_fd[0] >= 0 && ft_dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(STDOUT_FILENO);
		return (1);
	}
	if (not_subshell)
		return (exec_cmd(current, next, env));
	return (stt_exec_subshell(current, env));
}

static
pid_t	stt_exec_pipe(int *fd_read_end, t_token *current, t_token *next, t_env *env)
{
	int32_t	fd[2];
	pid_t	process_id;

	if (pipe(fd) == -1)
		return (ft_error("msh_pipe: ", NULL, -1));
	process_id = fork();
	if (process_id == 0)
	{
		close(fd[0]);				// Child needs the previous read end, not current
		if (*fd_read_end == -1)		// First run
			fd[0] = STDIN_FILENO;
		else
			fd[0] = *fd_read_end;
		_exit (stt_child(fd, current, next, env));
	}
	close(fd[1]);
	if (*fd_read_end >= 0)			// Closes previous read_end
		close(*fd_read_end);
	*fd_read_end = fd[0];			// And saves the current read end for the next child
	if (process_id < 0)
		return (ft_error("msh_fork: ", NULL, -1));
	else
		return (process_id);
}

static
pid_t	stt_exec_last(int *fd_read_end, t_token *current, t_token *next, t_env *env)
{
	int32_t	fd[2];
	pid_t	process_id;

	process_id = fork();
	if (*fd_read_end == -1)
		fd[0] = STDIN_FILENO;
	else
		fd[0] = *fd_read_end;
	fd[1] = STDOUT_FILENO;
	if (process_id == 0)
		_exit (stt_child(fd, current, next, env));
	if (*fd_read_end != -1)
	{
		close(*fd_read_end);
		*fd_read_end = -1;
	}
	if (process_id < 0)
		return (ft_error("msh_fork: ", NULL, -1));
	else
		return (process_id);
}

int	exec_pipeline(t_token *current, t_token *next, t_token *end, t_env *env)
{
	size_t	count;
	pid_t	cpid_list[FT_MAX_CHILDREN];
	pid_t	process_id;
	int		exit_status;
	int		fd_read_end;

	count = 0;
	fd_read_end = -1;
	while (next < end)
	{
		process_id = stt_exec_pipe(&fd_read_end, current, next, env);
		if (process_id > 0)
			cpid_list[count++] = process_id;
		current = next + 1;
		next = msh_next_delimiter(current, end, E_PIPE);
	}
	process_id = stt_exec_last(&fd_read_end, current, next, env);
	if (process_id > 0)
		cpid_list[count++] = process_id;
	exit_status = msh_wait_child(cpid_list, count);
	return (exit_status);
}
