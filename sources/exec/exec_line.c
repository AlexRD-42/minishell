/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/22 21:20:06 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

static
t_token	*stt_next_stu(t_token *start, t_token *end)
{
	ssize_t		pdepth;
	uint32_t	type;
	t_token		*tokens;

	pdepth = 0;
	tokens = start;
	while (tokens < end && (tokens->type & (E_END)) == 0)
	{
		type = tokens->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
		if (pdepth == 0 && (type & (E_AND | E_OR)))
			return (tokens);
		else if (pdepth < 0)
		{
			ft_error("Houston we have a problem", "", -1);
			return (NULL);
		}
		tokens++;
	}
	return (NULL);
}

// exec line needs to take a start and end parameter as well
// this is some recursion bullshit
// exec_line needs to loop through until it finds a AND, OR, PARENTHESIS
int	exec_line(t_token *start, t_token *end, t_env *env)
{
	int		status;
	t_token	*next;
	t_token	*current;

	if (start == end)
		return (0);
	current = start;
	next = stt_next_stu(current, end);
	while (next != NULL)
	{
		status = exec_stu(&(t_token_range){current, current, current, end}, env);
		if ((next->type & E_AND) && status != 0)
			return (status);
		if ((next->type & E_OR) && status == 0)
			return (status);
		current = next + 1;
		next = stt_next_stu(current, end);
	}
	return (exec_stu(&(t_token_range){current, current, current, end}, env));
}
