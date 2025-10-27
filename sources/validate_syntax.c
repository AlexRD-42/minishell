/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/07/13 11:18:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_syntax(t_shell *shell, int parenthesis_depth)
{
	t_token	*current;
	t_token	*previous;

	if (start_error(shell->tokens[0]))
		return (-1);
	parenthesis_depth = (shell->tokens[0].type == OPEN_PARENTHESIS);
	current = &shell->tokens[1];
	while (current->type != END)
	{
		previous = current - 1;
		if (parenthesis_error(*current, *previous, &parenthesis_depth))
			return (-1);
		if (redirect_error(*current, *previous))
			return (-1);
		if (operator_error(*current, *previous))
			return (-1);
		if (word_error(*current, *previous))
			return (-1);
		current++;
	}
	if (end_error(*(current - 1)))
		return (-1);
	if (parenthesis_depth > 0)
		return (write(2, "min: syntax error: unclosed '('\n", 39), -1);
	return (0);
}

static int	\
parenthesis_error(t_token current, t_token prev, int *parenthesis_depth)
{
	if (current.type & (OPEN_PARENTHESIS))
	{
		if (prev.type & (WORD | SINGLE_QUOTE | DOUBLE_QUOTE | APPEND
				| HEREDOC | REDIRECT_IN | REDIRECT_OUT | CLOSE_PARENTHESIS))
			return (syntax_error(current), -1);
		(*parenthesis_depth)++;
	}
	if (current.type & (CLOSE_PARENTHESIS))
	{
		if (prev.type & (PIPE | OR | AND | OPEN_PARENTHESIS | REDIRECT_IN
				| REDIRECT_OUT | APPEND | HEREDOC))
			return (syntax_error(current), -1);
		(*parenthesis_depth)--;
	}
	if (*parenthesis_depth < 0)
		return (syntax_error(current), -1);
	return (0);
}

static int	\
operator_error(t_token current, t_token prev)
{
	if (current.type & (PIPE | OR | AND))
	{
		if (prev.type & (PIPE | OR | AND | OPEN_PARENTHESIS | REDIRECT_IN
				| REDIRECT_OUT | APPEND | HEREDOC))
			return (syntax_error(current), -1);
	}
	return (0);
}

static int	\
redirect_error(t_token current, t_token prev)
{
	if (current.type & (REDIRECT_IN | REDIRECT_OUT | APPEND | HEREDOC))
	{
		if (prev.type & (PIPE | OR | AND | REDIRECT_IN | REDIRECT_OUT
				| APPEND | HEREDOC))
			return (syntax_error(current), -1);
	}
	return (0);
}

static int	\
word_error(t_token current, t_token prev)
{
	if (current.type & (WORD | SINGLE_QUOTE | DOUBLE_QUOTE))
	{
		if (prev.type & (CLOSE_PARENTHESIS))
			return (syntax_error(current), -1);
	}
	return (0);
}
