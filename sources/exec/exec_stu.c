/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stu.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:14:41 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 21:12:54 by adeimlin         ###   ########.fr       */
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
#include <stdio.h>
#include "msh_utils.h"
#include <stdlib.h>

static
int stt_wait_child(pid_t *cpid_list, size_t count)
{
	size_t	i;
	int		status;
	int		last_status;

	i = 0;
	last_status = 0;
	while (i < count)
	{
		waitpid(cpid_list[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

// Receives {start, start, start, end}
int	exec_stu(t_token_range *token, t_env *env)
{
	const int32_t	prev_fd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	size_t			count;
	pid_t			cpid_list[32];
	pid_t			process_id;
	int				rvalue;

	if (token->start == token->end)
		return (0);
	if (prev_fd[0] < 0 || prev_fd[1] < 0)
		return (ft_error("msh_dup: ", NULL, -1));	// Failed to save the state
	count = 0;
	while (token->current < token->end)
	{
		process_id = exec_pipe(token, env);
		if (process_id > 0)	// Does not save ID of parent executions
			cpid_list[count++] = process_id;
		if (token->next == NULL)
			break ;
		token->current = token->next + 1;
	}
	rvalue = stt_wait_child(cpid_list, count);
	if ((dup2(prev_fd[0], STDIN_FILENO) < 0) + (dup2(prev_fd[1], STDOUT_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);	// Restores fds
	close(prev_fd[0]);
	close(prev_fd[1]);
	return (rvalue);
}

// All of this in a loop where the next range is an Operator
// Need to open everything up until E_AND, E_OR, E_PIPE is found
// When a () is found, find its closing pair ignoring everything else
// Then find the next interval until a pipe or E_AND, E_OR, E_PIPE is found
// end is important for subshells

// 1) Open files, 2) Determine if we are calling a subshell
//	A) Subshell, B) Not Subshell
// If we are subshell, we call exec_subshell, which prepares stuff and calls exec_line again
// Which eventually will call exec_stu and return to the decision point
// If not subshell, proceed as regular