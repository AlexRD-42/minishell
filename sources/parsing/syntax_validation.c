/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/07/13 11:18:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	\
stt_syntax_check_parenthesis(t_token current, t_token prev, int *paren_depth)
{
	if (current.type & (FT_OPEN_PAREN))
	{
		if (prev.type & (FT_REDIR | FT_WORD | FT_CLOSE_PAREN))
			return (syntax_print_error(current), 1);
		(*paren_depth)++;
	}
	if (current.type & (FT_CLOSE_PAREN))
	{
		if (prev.type & (FT_OPERATOR | FT_REDIR | FT_OPEN_PAREN))
			return (syntax_print_error(current), 1);
		(*paren_depth)--;
	}
	if (*paren_depth < 0)
		return (syntax_print_error(current), 1);
	return (0);
}

static inline int	\
stt_syntax_check_operator(t_token current, t_token prev)
{
	if (current.type & (FT_OPERATOR))
	{
		if (prev.type & (FT_OPERATOR | FT_REDIR | FT_OPEN_PAREN))
			return (syntax_print_error(current), 1);
	}
	return (0);
}

static inline int	\
stt_syntax_check_redirect(t_token current, t_token prev)
{
	if (current.type & (FT_REDIR))
	{
		if (prev.type & (FT_REDIR))
			return (syntax_print_error(current), 1);
	}
	return (0);
}

static inline int	\
stt_syntax_check_word(t_token current, t_token prev)
{
	if (current.type & (FT_WORD))
	{
		if (prev.type & (FT_CLOSE_PAREN))
			return (syntax_print_error(current), 1);
	}
	return (0);
}

int	syntax_validation(t_shell *shell, int paren_depth)
{
	t_token	*current;
	t_token	*previous;

	if (syntax_check_start(shell->tokens[0]))
		return (-1);
	paren_depth = (shell->tokens[0].type == FT_OPEN_PAREN);
	current = &shell->tokens[1];
	while (current->type != FT_END)
	{
		previous = current - 1;
		if (stt_syntax_check_parenthesis(*current, *previous, &paren_depth))
			return (-1);
		if (stt_syntax_check_redirect(*current, *previous))
			return (-1);
		if (stt_syntax_check_operator(*current, *previous))
			return (-1);
		if (stt_syntax_check_word(*current, *previous))
			return (-1);
		current++;
	}
	if (syntax_check_end(*(current -1)))
		return (-1);
	if (paren_depth > 0)
		return (write(2, "mini: syntax error: unclosed '('\n", 33), -1);
	return (0);
}
