/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/23 17:53:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static
int	stt_syntax_check(uint32_t cur_type, uint32_t prev_type, int *pdepth)
{
	if (cur_type & (E_OPAREN))
	{
		if (prev_type & (E_REDIR | E_WORD | E_CPAREN))
			return (-1);
		(*pdepth)++;
	}
	else if (cur_type & (E_CPAREN))
	{
		if (prev_type & (E_OPERATOR | E_REDIR | E_OPAREN))
			return (-1);
		(*pdepth)--;
	}
	if (*pdepth < 0)
		return (-1);
	if ((cur_type & E_OPERATOR) && (prev_type & (E_OPERATOR | E_REDIR | E_OPAREN)))
		return (-1);	// operator
	if ((cur_type & E_WORD) && (prev_type & E_CPAREN))
		return (-1);	// word
	if ((cur_type & E_REDIR) && (prev_type & E_REDIR))
		return (-1);	// redirect
	return (0);
}

// Marcar o expand (primeiro nao eh)
// Marcar os fds
// Marcar simple command

size_t	syntax_validation(t_token *tokens)
{
	int	pdepth;

	if (tokens[0].type & (E_OPERATOR | E_CPAREN))
		return (SIZE_MAX);	// Print error first token bad
	pdepth = (tokens[0].type == E_OPAREN);
	while (!(tokens[1].type & (E_END)))
	{
		if (stt_syntax_check(tokens[1].type, tokens[0].type, &pdepth))
			return (SIZE_MAX);	// Print error at token ptr
		tokens++;
	}
	if (tokens[0].type & (E_OPERATOR | E_REDIR | E_OPAREN))
		return (SIZE_MAX);	// Print error 
	if (pdepth > 0)
		return (SIZE_MAX);	// Unclosed (
	return (0);
}
