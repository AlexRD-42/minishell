/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 20:50:06 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 21:15:18 by adeimlin         ###   ########.fr       */
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
t_token	*stt_next_pipe(t_token *tokens, t_token *end)
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
		if (pdepth < 0)
			ft_error("Houston we have a problem", "", -1);
		tokens++;
	}
	return (NULL);
}

static
pid_t	stt_fork(int32_t *fd, pid_t process_id, t_token_range *token, t_env *env)
{
	int			rvalue;
	const bool	not_subshell = !(token->current->type & E_OPEN_PAREN);
	const bool	is_child = (process_id == 0);

	rvalue = 0;
	if (token->next != NULL)
	{
		close(fd[!is_child]);
		rvalue = dup2(fd[is_child], is_child);
		close(fd[is_child]);
	}
	if (rvalue == -1)
		return (ft_error("msh_dup2: ", NULL, -1));
	if (is_child == 0)
		return (process_id);
	msh_open_files(token->current, token->next, env);
	if (not_subshell)
		_exit (exec_cmd(token->current, env));
	else
		_exit (exec_subshell(token->current, env));
}

pid_t	exec_pipe(t_token_range *token, t_env *env)
{
	int32_t		fd[2];
	const bool	not_subshell = !(token->current->type & E_OPEN_PAREN);
	pid_t		process_id;

	process_id = 0;
	token->next = stt_next_pipe(token->current, token->end);
	if (not_subshell && token->current == token->start && token->next == NULL
			&& exec_simple(token->start, token->end, env) == 1)
		return (0);	// Was executed in the parent
	if (token->next != NULL && pipe(fd) == -1)
		return (ft_error("msh_pipe: ", NULL, -1));
	process_id = fork();
	if (process_id >= 0)
		return (stt_fork(fd, process_id, token, env));
	if (token->next != NULL)
	{
		close(fd[1]);
		close(fd[0]);
	}
	return (ft_error("msh_fork: ", NULL, -1));
}
