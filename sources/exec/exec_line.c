/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 15:36:14 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static
t_token	*stt_findstu_sep(t_token *tokens)
{
	t_token	*current;
	int		in_paren;

	in_paren = 0;
	current = tokens;
	while (!(current->type & (E_END)))
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

static
int	stt_expand_asterisk(const char *str, size_t length)
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

// marca como file, e inicializa os fds, abre os heredocs
static
void	stt_set_fds(t_token *tokens, t_env *env)
{
	while (!(tokens[0].type & (E_END)))
	{
		if (tokens[0].type & (E_HRDOC))
		{
			tokens[1].type = E_LIMITER;
			if (!stt_has_quote(tokens[1].ptr, tokens[1].length))
				tokens[1].type |= E_EXPAND;
			tokens[0].fd[0] = heredoc(tokens + 1, env);
		}
		if (tokens[0].type & (E_REDIR))
		{
			tokens[0].fd[0] = -1;
			tokens[0].fd[1] = -1;
			tokens[1].type = (E_FILE | tokens[0].type);
		}
		else if (tokens[0].type & (E_WORD | E_FILE))
		{
			if (stt_expand_asterisk(tokens[0].ptr, tokens[0].length))	// Change to parsing
				tokens[0].type |= E_EXPAND;
		}
		tokens += 1 + !!(tokens[0].type & E_REDIR);
	}
}

int	exec_line(t_token *tokens, t_env *env)
{
	t_token	*current;
	int		rvalue;

	if (tokens[0].type & (E_END))
		return (0);
	stt_set_fds(tokens, env);
	current = stt_findstu_sep(tokens);
	rvalue = exec_stu(tokens, env);
	if (current->type & E_END)
		return (att_exit(rvalue, true));
	while (current->type & (E_AND | E_OR))
	{
		stt_findstu_sep(current + 1);
		if ((current->type & E_OR) && rvalue)
			rvalue = exec_stu(current + 1, env);
		else if ((current->type & E_AND) && !rvalue)
			rvalue = exec_stu(current + 1, env);
		while (!((++current)->type & E_STU_END))
			current++;
		att_exit(rvalue, true);
	}
	return (rvalue);
}
