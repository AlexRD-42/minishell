/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:15:52 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 19:01:17 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

void	msh_wait_child(pid_t *cpid_list, size_t count, t_env *env)
{
	size_t	i;
	int		status;

	i = 0;
	while (i < count)
	{
		if (cpid_list[i] > 0)
			waitpid(cpid_list[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				env->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				env->exit_status = 128 + WTERMSIG(status);
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
