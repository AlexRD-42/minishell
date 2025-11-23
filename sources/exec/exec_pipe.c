/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 20:50:06 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 16:30:23 by adeimlin         ###   ########.fr       */
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

// static const struct sigaction
// sa = {.sa_handler = SIG_DFL, .sa_mask = {0},.sa_flags = 0};
static
pid_t	stt_child(int32_t *fd, bool not_subshell, t_token_range *token, t_env *env)
{
	int	rvalue;

	sigaction(SIGINT, env->sig_dfl, NULL);
	sigaction(SIGQUIT, env->sig_dfl, NULL);
	sigaction(SIGWINCH, env->sig_dfl, NULL);
	sigaction(SIGTTOU, env->sig_dfl, NULL);
	sigaction(SIGTTIN, env->sig_dfl, NULL);
	rvalue = 0;
	if (token->next != NULL)
	{
		close(fd[0]);
		rvalue = dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (rvalue == -1)
			return (ft_error("msh_dup2: ", NULL, -1));
	}
	msh_open_files(token->current, token->next, env);
	if (not_subshell)
		return (exec_cmd(token->current, env));
	else
		return (exec_subshell(token->current, env));
}

// When it is a pipeline, parent dups its STDIN to childs STDOUT
static
pid_t	stt_parent(int32_t *fd, pid_t process_id, t_token_range *token)
{
	int	rvalue;

	rvalue = 0;
	if (token->next != NULL)
	{
		close(fd[1]);
		rvalue = dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		if (rvalue == -1)
			return (ft_error("msh_dup2: ", NULL, -1));
	}
	return (process_id);
}

pid_t	exec_pipe(t_token_range *token, t_env *env)
{
	int32_t		fd[2];
	const bool	not_subshell = !(token->current->type & E_OPEN_PAREN);
	pid_t		process_id;

	process_id = 0;
	token->next = msh_next_delimiter(token->current, token->end, E_PIPE);
	if (not_subshell && token->current == token->start && token->next == NULL
			&& exec_simple(token->start, token->end, env) == 1)
		return (0);	// Was executed in the parent
	if (token->next != NULL && pipe(fd) == -1)
		return (ft_error("msh_pipe: ", NULL, -1));
	process_id = fork();
	if (process_id == 0)
		_exit (stt_child(fd, not_subshell, token, env));
	if (process_id > 0)
		return (stt_parent(fd, process_id, token));
	if (token->next != NULL)
	{
		close(fd[1]);
		close(fd[0]);
	}
	return (ft_error("msh_fork: ", NULL, -1));
}
