/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/08 10:08:26 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// State: XOR TOGGLE
static
t_token	*stt_findstu_sep(t_token *tokens)
{
	t_token	*current;
	t_token	*sep;
	int		in_paren;

	sep = NULL;
	in_paren = 0;
	current = tokens;
	while (current->type != (FT_END))
	{
		if (current->type & (FT_OPEN_PAREN))
			in_paren++;
		else if (current->type & (FT_CLOSE_PAREN))
			in_paren--;
		if (!in_paren && current->type & (FT_AND | FT_OR))
		{
			sep = current;
			break ;
		}
		current++;
	}
	if (!sep)
		return (current);
	return (sep);
}

static const
char	*stt_has_quote(const char *str, size_t length)
{
	const char	*end = str + length;

	while (str < end && *str != 0)
	{
		if (*str == '\'' || *str == '"')
			return (str);
		str++;
	}
	return (NULL);
}

static int	stt_expand_asterisk(char *str, size_t length)
{
	const char	*end = str + length;
	char		quote;

	quote = 0;
	while (str < end && *str != 0)
	{
		if (*str == '\'' || *str == '"')
		{
			quote = *str;
			while (str < end && *str && *str != quote)
				str++;
		}
		else if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}

static void	stt_set_fds(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current->type != (FT_END))
	{
		if (current->type & (FT_HRDOC))
		{
			(current + 1)->type = FT_LIMITER;
			if (!stt_has_quote((current + 1)->ptr, (current + 1)->length))
				(current + 1)->type |= FT_EXPAND;
			current->fds[0] = heredoc(current + 1);
		}
		else if (current->type & (FT_REDIR))
		{
			current->fds[0] = -1;
			current->fds[1] = -1;
			(current + 1)->type = FT_FILE;
		}
		else if (current->type & (FT_WORD | FT_FILE))
		{
			if (stt_expand_asterisk(current->ptr, current->length))
				current->type |= FT_EXPAND;
		}
		current++;
	}
}

int	execute(t_shell *shell)
{
	t_token	*current;
	int		return_value;

	if (shell->tokens[0].type & (FT_END))
		return ;
	stt_set_fds(shell->tokens);
	current = stt_findstu_sep(shell->tokens);
	return_value = exec_stu(shell->tokens);
	if (current->type & FT_END)
		return (return_value);
	while (current->type & (FT_AND | FT_OR))
	{
		if ((current->type & FT_OR) && return_value)
			return_value = exec_stu(current + 1);
		if ((current->type & FT_AND) && !return_value)
			return_value = exec_stu(current + 1);
		current = stt_findstu_sep(current + 1);
	}
	return (return_value);
}
