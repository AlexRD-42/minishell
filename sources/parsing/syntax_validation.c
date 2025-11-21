/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 14:46:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static
int	stt_syntax_check(uint32_t cur_type, uint32_t prev_type, int *pdepth)
{
	if (cur_type & (E_OPEN_PAREN))
	{
		if (prev_type & (E_REDIR | E_WORD | E_CLOSE_PAREN))
			return (-1);
		(*pdepth)++;
	}
	else if (cur_type & (E_CLOSE_PAREN))
	{
		if (prev_type & (E_OPERATOR | E_REDIR | E_OPEN_PAREN))
			return (-1);
		(*pdepth)--;
	}
	if (*pdepth < 0)
		return (-1);
	if ((cur_type & E_OPERATOR) && (prev_type & (E_OPERATOR | E_REDIR | E_OPEN_PAREN)))
		return (-1);	// operator
	if ((cur_type & E_WORD) && (prev_type & E_CLOSE_PAREN))
		return (-1);	// word
	if ((cur_type & E_REDIR) && (prev_type & E_REDIR))
		return (-1);	// redirect
	return (0);
}

// Marcar o expand (primeiro nao eh)
// Marcar os fds
// Marcar simple command

int	syntax_validation(t_token *tokens)
{
	int	pdepth;

	if (tokens[0].type & (E_OPERATOR | E_CLOSE_PAREN))
		return (-1);	// Print error first token bad
	pdepth = (tokens[0].type == E_OPEN_PAREN);
	while (!(tokens[1].type & (E_END)))
	{
		if (stt_syntax_check(tokens[1].type, tokens[0].type, &pdepth))
			return (-1);	// Print error at token ptr
		tokens++;
	}
	if (tokens[0].type & (E_OPERATOR | E_REDIR | E_OPEN_PAREN))
		return (-1);	// Print error 
	if (pdepth > 0)
		return (-1);	// Unclosed (
	return (0);
}
