/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:15:52 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 18:06:36 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <wait.h>
#include <errno.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include "msh_utils.h"

int	msh_wait_child(pid_t *cpid_list, size_t count)
{
	size_t	i;
	int		status;
	pid_t	cpid;
	size_t	retries;

	i = 0;
	cpid = 0;	// Initializes cpid in case count == 0
	while (i < count)
	{
		retries = FT_SYSCALL_RETRIES;
		cpid = waitpid(cpid_list[i], &status, 0);
		while (cpid == -1 && errno == EINTR && retries-- > 0)
			cpid = waitpid(cpid_list[i], &status, 0);
		if (cpid == -1)
			ft_error("msh_waitpid: ", NULL, 1);	// Log the error but continue
		i++;
	}
	if (cpid > 0 && WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (cpid > 0 && WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

t_token	*msh_next_delimiter(t_token *start, t_token *end, uint32_t delimiter)
{
	ssize_t		pdepth;
	uint32_t	type;

	pdepth = 0;
	while (start < end && !(start->type & E_END))
	{
		type = start->type;
		pdepth += !!(type & E_OPAREN) - !!(type & E_CPAREN);
		if (pdepth == 0 && (type & delimiter))
			return (start);
		if (pdepth < 0)
		{
			ft_error("msh_unexpected: Negative parenthesis depth", "", 1);
			return (end);
		}
		start++;
	}
	return (end);
}

// Review: Maybe this should also take an end pointer (all the callers have end)
ssize_t	msh_build_argv(t_token *start, t_token *end, t_env *env, t_vecp *argv)
{
	ssize_t	rvalue;
	t_token	*tokens;

	tokens = start;
	while (tokens < end && argv->count < argv->max_count - 1)
	{
		if (tokens->type & E_WORD)
		{
			rvalue = expand_token(*tokens, env, argv);
			if (rvalue < 0)
			{
				if (rvalue == -4)
					ft_error("msh_memory: argv out of bounds", "", -1);
				return (rvalue);
			}
		}
		tokens++;
	}
	argv->ptr[argv->count] = NULL;
	return ((ssize_t) argv->count);
}
