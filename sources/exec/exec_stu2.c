/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stu2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:14:41 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 17:51:14 by adeimlin         ###   ########.fr       */
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
#include <signal.h>

static
int32_t	stt_pipe(int32_t *pipe_fd)
{
	int32_t	fd[2];

	if (pipe(fd) == -1)
			return (ft_error("msh_pipe: ", NULL, -1));
	if (pipe_fd[1] == -1)	// No redirs to out
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			return (ft_error("msh_dup2: ", NULL, -1));
		close(fd[1]);
		return (fd[0]);	// Parent needs to dup this to its stdin AFTER forking
	}
	return (0);
}

static
t_token	*stt_next_pipe(t_token *tokens, t_token *end, t_env *env, int32_t *pipe_fd)
{
	ssize_t		pdepth;
	uint32_t	type;

	pdepth = 0;
	while (tokens < end && !(tokens->type & E_END))
	{
		type = tokens->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
		if (pdepth == 0 && (type & E_PIPE))
			return (tokens);
		if ((type & E_REDIR) && pdepth == 0 && msh_apply_redir(tokens, env, pipe_fd) < 0)
			return (NULL);
		if (pdepth < 0)
			ft_error("Houston we have a problem", "", -1);
		tokens++;
	}
	return (NULL);
}

static
int stt_exec_simple(t_token *tokens, const int32_t *prev_fd, t_env *env)
{
	char	*arg_ptr[FT_ARG_COUNT];
	char	buffer[FT_ARG_SIZE];
	t_vecp  argv;
	int	 	rvalue;

	argv = (t_vecp){{buffer, buffer + sizeof(buffer), buffer}, 0, FT_ARG_COUNT, arg_ptr};
	if (msh_build_argv(tokens, env, &argv) < 0)
		return (1);
	rvalue = msh_dispatch(&argv, env);
	if ((dup2(prev_fd[0], STDIN_FILENO) < 0) + (dup2(prev_fd[1], STDOUT_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);	// Restores fds
	close(prev_fd[0]);	// Do we throw error messages for close?
	close(prev_fd[1]);
	return (rvalue);
}

static
pid_t	stt_fork(t_token *current, t_token *next, t_token *par_ptr, t_env *env)
{
	pid_t	process_id;

	process_id = fork();
	if (process_id < 0)
		return (ft_error("msh_fork: ", NULL, -1));
	if (process_id > 0)
		return (process_id);	// Process should not return after this
	if (par_ptr == NULL)
		_exit (exec_cmd(current, env));
	else
		_exit (exec_subshell(par_ptr, next, env));
}

// Decide to fork early, that way the child always does the dups!
static
pid_t	stt_exec_pipe(t_token_range *token, t_env *env)
{
	const int32_t	prev_fd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	int32_t			next_fd[2];
	int32_t			new_fd;
	t_token			*par_ptr;
	pid_t			process_id;

	if (prev_fd[0] < 0 || prev_fd[1] < 0)
		return (ft_error("msh_dup: ", NULL, -1));	// Failed to save the state
	process_id = 0;
	par_ptr = NULL;
	if (token->current->type & E_OPEN_PAREN)
		par_ptr = token->current;
	token->next = stt_next_pipe(token->current, token->end, env, next_fd);
	new_fd = stt_pipe(next_fd);
	if (!token->next && !par_ptr && token->current == token->start && msh_mutates_state(token->current, token->end))
	{
		stt_exec_simple(token->current, prev_fd, env);
		return (0);
	}
	else
		process_id = stt_fork(token->current, token->next, par_ptr, env);
	dup2(new_fd, prev_fd[1]);
	return (process_id);
}

// Receives {start, start, start, end}
int	exec_stu(t_token_range *token, t_env *env)
{
	size_t		i;
	pid_t		child_pids[32];
	pid_t		process_id;

	if (token->start == token->end)
		return (0);
	i = 0;
	while (token->current < token->end)
	{
		process_id = stt_exec_pipe(token, env);
		if (process_id > 0)	// Does not save ID of parent executions
			child_pids[i++] = process_id;
		if (token->next == NULL)
			break ;
		token->current = token->next + 1;
	}
	// Close STDOUT
	// Wait for children to finish
	// Close STDIN
	return (0);
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