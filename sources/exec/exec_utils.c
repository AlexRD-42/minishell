/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:15:52 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 11:34:31 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <wait.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

// Review :
// Track whether waitpid actually succeeded for the last child and only inspect status in that case. 
// On error, you probably want to set env->exit_status to some error value explicitly.
void	msh_wait_child(pid_t *cpid_list, size_t count, t_env *env)
{
	size_t	i;
	int		last_status;
	pid_t	cpid;

	i = 0;
	while (i < count)
	{
		cpid = waitpid(cpid_list[i], &last_status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(last_status))
				env->exit_status = WEXITSTATUS(last_status);
			else if (WIFSIGNALED(last_status))
				env->exit_status = 128 + WTERMSIG(last_status);
		}
		i++;
	}
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

// To do: Check if expand_token null terminates the argument (it should)
// To do: Print error messages for exceeded count
// Review: Maybe this should also take an end pointer (all the callers have end)
ssize_t	msh_build_argv(t_token *token, t_env *env, t_vecp *argv)
{
	ssize_t	rvalue;

	while ((token->type & E_CMD_END) == 0 && argv->count < argv->max_count - 1)
	{
		if (token->type & E_WORD)
		{
			rvalue = expand_token(*token, env, argv);
			if (rvalue < 0)
			{
				if (rvalue == -4)
					ft_error("msh_memory: argv out of bounds", "", -1);
				return (rvalue);
			}
		}
		token++;
	}
	argv->ptr[argv->count] = NULL;
	return ((ssize_t) argv->count);
}
