/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/10 17:12:16 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	\
stt_syntax_check_parenthesis(t_token current, t_token prev, int *paren_depth)
{
	if (current.type & (E_OPEN_PAREN))
	{
		if (prev.type & (E_REDIR | E_WORD | E_CLOSE_PAREN))
			return (syntax_print_error(current), 1);
		(*paren_depth)++;
	}
	if (current.type & (E_CLOSE_PAREN))
	{
		if (prev.type & (E_OPERATOR | E_REDIR | E_OPEN_PAREN))
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
	if (current.type & (E_OPERATOR))
	{
		if (prev.type & (E_OPERATOR | E_REDIR | E_OPEN_PAREN))
			return (syntax_print_error(current), 1);
	}
	return (0);
}

static inline int	\
stt_syntax_check_redirect(t_token current, t_token prev)
{
	if (current.type & (E_REDIR))
	{
		if (prev.type & (E_REDIR))
			return (syntax_print_error(current), 1);
	}
	return (0);
}

static inline int	\
stt_syntax_check_word(t_token current, t_token prev)
{
	if (current.type & (E_WORD))
	{
		if (prev.type & (E_CLOSE_PAREN))
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
	paren_depth = (shell->tokens[0].type == E_OPEN_PAREN);
	current = &shell->tokens[1];
	while (current->type != E_END)
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
