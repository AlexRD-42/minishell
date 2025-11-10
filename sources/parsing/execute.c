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
	t_token	*root;
	int		in_paren;

	root = NULL;
	in_paren = 0;
	current = tokens;
	while (current->type != END)
	{
		if (current->type & OPEN_PAREN)
			in_paren++;
		else if (current->type & CLOSE_PAREN)
			in_paren--;
		if (!in_paren && current->type & (AND | OR))
			root = current;
		current++;
	}
	return (root);
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

static
void	stt_set_fds(t_token *tokens)
{
	t_token	*current;
	int		expand;

	expand = 1;
	current = &tokens[0];	// ??
	while (current->type != END)
	{
		if (current->type & HEREDOC)
		{
			if (stt_has_quote((current + 1)->ptr, (current + 1)->length))
				expand = 0;
			current->fds[0] = heredoc(current + 1, expand);
			(current + 1)->type = LIMITER;
		}
		else if (current->type & (REDIR))
		{
			current->fds[0] = -1;
			current->fds[1] = -1;
			(current + 1)->type = FILE;
		}
		current++;
	}
}

int	execute(t_shell *shell)
{
	t_token	*current;
	int		return_value;

	if (shell->tokens[0].type & END)
		return ;
	stt_set_fds(shell->tokens);
	current = stt_findstu_sep(shell->tokens);
	return_value = exec_stu(shell->tokens); 
	if (!current)
		return (return_value);
	while(current->type & (AND | OR))
	{
		if ((current->type & OR) && return_value)
			return_value = exec_stu(current + 1);
		if ((current->type & AND) && !return_value)
			return_value = exec_stu(current + 1);
		current = stt_findstu_sep(current + 1);
		if (!current)
			break ;
	}
	return (return_value);
}
