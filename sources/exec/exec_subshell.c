/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:41:55 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 21:08:05 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_defines.h"

// Always exits upon return
int	exec_subshell(t_token *start, t_env *env)
{
	t_token		*end;
	size_t		pdepth;
	uint32_t	type;

	start += !!(start->type & E_OPEN_PAREN);	// Could just do start++
	end = start;
	type = end->type;
	pdepth = 1 - !!(type & E_CLOSE_PAREN);
	while (pdepth != 0)		// Dangerous if pre-validation is wrong but good for debugging
	{
		end++;
		type = end->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
	}
	_exit (exec_line(start, end - 1, env));	// Double check the math here
}
