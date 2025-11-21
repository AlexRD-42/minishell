/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:41:55 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 20:58:16 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"

static
t_token	*stt_get_end(t_token *tokens)
{
	size_t		pdepth;
	uint32_t	type;

	type = tokens->type;
	pdepth = 1 - !!(type & E_CLOSE_PAREN);
	while (pdepth != 0)		// Dangerous if pre-validation is wrong but good for debugging
	{
		tokens++;
		type = tokens->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
	}
	return (tokens);
}

// Always exits upon return
void	exec_subshell(t_token *tokens, t_env *env)
{
	t_token	*start;
	t_token	*end;
	int		rvalue;

	start = tokens + 1;
	end = stt_get_end(start);
	rvalue = exec_stu(start, end, env);
	_exit(rvalue);
}
