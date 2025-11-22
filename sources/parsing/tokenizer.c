/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 19:37:28 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_utils.h"
#include <stdio.h>

static const
char	*stt_token_quote_handler(t_token *token, const char *end, char quote)
{
	end++;
	while (*end && *end != quote)
		end++;
	if (!*end)
	{
		token->type = E_ERROR;
		write(2, "minishell: syntax error: unclosed quote\n", 40);
		return (NULL);
	}
	end++;
	return (end);
}

// Aqui eh possivel criar uma LUT que sempre sera usada, do tipo:
// 	static const uint8_t	lut[256] = {
// ['\t'] = 1, ['\n'] = 1, ['\v'] = 1, ['\f'] = 1, ['\r'] = 1};
static
size_t	stt_token_word_handler(t_token *token, const char *str)
{
	const char		*start;
	char		*lookup_table;
	const char	*end;
	char		quote;

	start = str;
	lookup_table = "\t\n\v\f\r |&();<>'\"";
	end = ft_strfind(start, lookup_table, 1);
	if (!end)
		end = start + ft_strlen(start);
	while (*end == '"' || *end == '\'')
	{
		quote = *end;
		end = stt_token_quote_handler(token, end, quote);
		if (!end)
			return (0);		// Erro?
		end = ft_strfind(end, lookup_table, 1);
		if (!end)
			break ;
	}
	if (!end)
		end = start + ft_strlen(start);
	token->type = E_WORD;
	return ((size_t)(end - start));
}

static
size_t	stt_token_finder(t_token *token, const char *str)
{
	if ((str[0] == '&' && str[1] != '&') || (str[0] == ';')
		|| (str[0] == '>' && str[1] == '>' && str[2] == '>')
		|| (str[0] == '<' && str[1] == '<' && str[2] == '<'))
		return (0);
	if (str[0] == '>' && str[1] == '>')
		token->type = E_APPND;
	else if (str[0] == '<' && str[1] == '<')
		token->type = E_HRDOC;
	else if (str[0] == '&' && str[1] == '&')
		token->type = E_AND;
	else if (str[0] == '|' && str[1] == '|')
		token->type = E_OR;
	else if (str[0] == '|')
		token->type = E_PIPE;
	else if (str[0] == '(')
		token->type = E_OPEN_PAREN;
	else if (str[0] == ')')
		token->type = E_CLOSE_PAREN;
	else if (str[0] == '>')
		token->type = E_REDIR_OUT;
	else if (str[0] == '<')
		token->type = E_REDIR_IN;
	else
		return (stt_token_word_handler(token, str));
	return (1 + ((token->type & (E_APPND | E_HRDOC | E_AND | E_OR)) != 0));
}

static const
char	*stt_get_next_token(t_token *token, const char *str)
{
	const char	*ostr = str;

	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == 0 || *str == '#')
	{
		token->ptr = ostr;
		token->type = E_END;
		token->length = (size_t)(str - ostr);
		return (str);
	}
	token->ptr = str;
	token->type = E_UNSET;
	token->length = stt_token_finder(token, str);
	if (token->length == 0)
	{
		token->type = E_ERROR;
		write(2, "minishell: syntax error near not implemented token\n", 51);
		return (NULL);
	}
	return (str + token->length);
}

// When it is a heredoc, we mark as expand if it has no quotes
// When it is a REDIR, we initialize FDs to -1
size_t	tokenize(t_token *tokens, const char *input)
{
	size_t	count;

	count = 0;
	while (true)
	{
		if (count >= FT_TOKEN_COUNT - 1)
		{
			write(2, "too many tokens\n", 16);			// ERROR_MSG
			tokens[FT_TOKEN_COUNT - 1].type = E_END;
			return (SIZE_MAX);
		}
		input = stt_get_next_token(tokens + count, input);
		if (tokens[count].type & (E_END))
			break ;
		if (tokens[count].type & (E_ERROR))			// ?
			return (SIZE_MAX);
		count++;
	}
	return (count);
}
