/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 15:31:23 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/24 20:39:41 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>
#include <signal.h>

extern volatile sig_atomic_t	g_signal;

static uint32_t	stt_expan(t_token token, int heredoc)
{
	const char	*end = token.ptr + token.length;
	char		quote;
	const char	*str;

	quote = 0;
	str = token.ptr;
	while (str < end && *str)
	{
		if (*str == '\'' || *str == '"')
		{
			if (heredoc)
				return (0);
			quote = *str;
			while (str < end && *str && *str != quote)
				str++;
		}
		else if (!heredoc && *str == '*')
			return (E_EXPAND);
		str++;
	}
	if (heredoc)
		return (E_EXPAND);
	return (0);
}

static void	stt_prepare_tokens(t_token *tokens)
{
	t_token	*cmd;

	cmd = NULL;
	while (!(tokens[0].type & (E_END)))
	{
		if (tokens[0].type & (E_REDIR_IN | E_REDIR_OUT | E_APPND))
		{
			tokens[0].fd[0] = -1;
			tokens[0].fd[1] = -1;
			tokens[1].type = E_FILENAME | stt_expan(tokens[1], 0);
		}
		else if (tokens[0].type & (E_WORD))
		{
			if (cmd && stt_expan(tokens[0], 0))
				tokens[0].type |= E_EXPAND;
			cmd = tokens;
		}
		if (tokens[0].type & E_OPERATOR)
			cmd = NULL;
		tokens += 1 + !!(tokens[0].type & E_REDIR);
	}
}

static size_t	stt_handle_heredocs(t_token *tokens, t_env *env)
{
	bool	must_expand;

	while (!(tokens[0].type & (E_END)))
	{
		if (tokens[0].type & (E_HRDOC) && tokens[1].type & E_WORD)
		{
			if (stt_expan(tokens[1], 1))
				must_expand = true;
			else
				must_expand = false;
			tokens[1].type = E_LIMITER;
			tokens[0].fd[0] = heredoc(tokens[1].ptr, tokens[1].length, must_expand, env);
			tokens[0].fd[1] = -1;
			if (g_signal == SIGINT)
			{
				env->exit_status = 130;
				g_signal = 0;
				break ;
			}
		}
		tokens++;
	}
	return (0);
}

size_t	parsing(t_token *tokens, char *input, t_token **end, t_env *env)
{
	if (tokenize(tokens, input, end) == SIZE_MAX)
		return (SIZE_MAX);
	if (stt_handle_heredocs(tokens, env) == SIZE_MAX)
		return (SIZE_MAX);
	if (syntax_validation(tokens, *end) == SIZE_MAX)
		return (SIZE_MAX);
	stt_prepare_tokens(tokens);
	return (0);
}
