/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 20:50:06 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 20:59:14 by adeimlin         ###   ########.fr       */
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

// Always exits upon return
static
int	stt_exec_subshell(t_token *start, t_env *env)
{
	t_token		*end;
	size_t		pdepth;
	uint32_t	type;

	start += !!(start->type & E_OPAREN);
	end = start;
	type = end->type;
	pdepth = 1 - !!(type & E_CPAREN);
	while (pdepth != 0)
	{
		end++;
		type = end->type;
		pdepth += !!(type & E_OPAREN) - !!(type & E_CPAREN);
	}
	_exit (exec_line(start, end, env));
}

// static const struct sigaction
// sa = {.sa_handler = SIG_DFL, .sa_mask = {0},.sa_flags = 0};
static
pid_t	stt_child(int32_t *fd, t_token *current, t_token *next, t_env *env)
{
	int			rvalue;
	const bool	not_subshell = !(current->type & E_OPAREN);

	sigaction(SIGINT, env->sig_dfl, NULL);
	sigaction(SIGQUIT, env->sig_dfl, NULL);
	sigaction(SIGWINCH, env->sig_dfl, NULL);
	rvalue = 0;
	if (env->fd_tmp != -1)
		close(env->fd_tmp);
	if (fd[0] != -1 || fd[1] != -1)
	{
		close(fd[0]);
		rvalue = dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (rvalue == -1)
			return (ft_error("msh_dup2: ", NULL, 1));
	}
	if (msh_open_files(current, next, env) >= 4)
		return (1);
	if (not_subshell)
		return (exec_cmd(current, env));
	else
		return (stt_exec_subshell(current, env));
}

// 	Parent: STDIN  > READ_END of pipe
// 	Child:	STDOUT > WRITE_END of pipe
static
pid_t	stt_parent(int32_t *fd, pid_t process_id)
{
	int	rvalue;

	rvalue = 0;
	if (fd[0] != -1 || fd[1] != -1)
	{
		close(fd[1]);
		rvalue = dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (rvalue == -1)
			return (ft_error("msh_dup2: ", NULL, -1));
	}
	return (process_id);
}

static
pid_t	stt_exec_pipe(t_token *current, t_token *next, t_token *end, t_env *env)
{
	int32_t		fd[2];
	pid_t		process_id;

	fd[0] = -1;
	fd[1] = -1;
	process_id = 0;
	if (next != end && pipe(fd) == -1)
		return (ft_error("msh_pipe: ", NULL, -1));
	process_id = fork();
	if (process_id == 0)
		_exit (stt_child(fd, current, next, env));
	if (process_id > 0)
		return (stt_parent(fd, process_id));
	if (fd[0] != -1 || fd[1] != -1)
	{
		close(fd[1]);
		close(fd[0]);
	}
	return (ft_error("msh_fork: ", NULL, -1));
}

int	exec_pipeline(t_token *current, t_token *next, t_token *end, t_env *env)
{
	size_t	count;
	pid_t	cpid_list[FT_MAX_CHILDREN];
	pid_t	process_id;
	int		exit_status;

	count = 0;
	cpid_list[count++] = stt_exec_pipe(current, next, end, env);
	current = next + 1;
	while (current < end)
	{
		next = msh_next_delimiter(current, end, E_PIPE);
		process_id = stt_exec_pipe(current, next, end, env);
		if (process_id > 0)
			cpid_list[count++] = process_id;
		current = next + 1;
	}
	exit_status = msh_wait_child(cpid_list, count);
	return (exit_status);
}
