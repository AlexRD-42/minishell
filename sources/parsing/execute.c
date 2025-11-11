/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/11 15:12:51 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// State: XOR TOGGLE
static
t_token	*stt_findstu_sep(t_token *tokens)
{
	t_token	*current;
	int		in_paren;

	in_paren = 0;
	current = tokens;
	while (current->type != (E_END))
	{
		if (current->type & (E_OPEN_PAREN))
			in_paren++;
		else if (current->type & (E_CLOSE_PAREN))
			in_paren--;
		if (!in_paren && current->type & (E_AND | E_OR))
		{
			current->type |= E_STU_END;
			return (current);
		}
		current++;
	}
	current->type |= E_STU_END;
	return (current);
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
	while (current->type != (E_END))
	{
		if (current->type & (E_HRDOC))
		{
			(current + 1)->type = E_LIMITER;
			if (!stt_has_quote((current + 1)->ptr, (current + 1)->length))
				(current + 1)->type |= E_EXPAND;
			current->fd[0] = heredoc(current + 1);
		}
		else if (current->type & (E_REDIR))
		{
			current->fd[0] = -1;
			current->fd[1] = -1;
			(current + 1)->type = E_FILE | current->type;
		}
		else if (current->type & (E_WORD | E_FILE))
		{
			if (stt_expand_asterisk(current->ptr, current->length))
				current->type |= E_EXPAND;
		}
		current++;
	}
}

int	execute(t_shell *shell)
{
	t_token	*current;
	int		return_value;

	if (shell->tokens[0].type & (E_END))
		return ;
	stt_set_fds(shell->tokens);
	current = stt_findstu_sep(shell->tokens);
	return_value = exec_stu(shell->tokens);
	if (current->type & E_END)
		return (return_value);
	while (current->type & (E_AND | E_OR))
	{
		stt_findstu_sep(current + 1);
		if ((current->type & E_OR) && return_value)
			return_value = exec_stu(current + 1);
		else if ((current->type & E_AND) && !return_value)
			return_value = exec_stu(current + 1);
		while (current->type != E_STU_END)
			current++;
	}
	return (return_value);
}
