/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/04 16:41:10 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const
char	*stt_token_quote_handler(t_token *token, const char *end, char quote)
{
	end++;
	while (*end && *end != quote)
		end++;
	if (!*end)
	{
		token->type = FT_ERROR;
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
size_t	stt_token_word_handler(t_token *token, char *str)
{
	char		*start;
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
			return (0);// Erro?
		end = ft_strfind(end, lookup_table, 1);
		if (!end)
			break ;
	}
	if (!end)
		end = start + ft_strlen(start);
	token->type = FT_WORD;
	return ((size_t)(end - start));
}

static
size_t	stt_token_finder(t_token *token, char *str)
{
	if ((str[0] == '&' && str[1] != '&') || (str[0] == ';')
		|| (str[0] == '>' && str[1] == '>' && str[2] == '>')
		|| (str[0] == '<' && str[1] == '<' && str[2] == '<'))
		return (0);
	if (str[0] == '>' && str[1] == '>')
		token->type = FT_APPND;
	else if (str[0] == '<' && str[1] == '<')
		token->type = FT_HRDOC;
	else if (str[0] == '&' && str[1] == '&')
		token->type = FT_AND;
	else if (str[0] == '|' && str[1] == '|')
		token->type = FT_OR;
	else if (str[0] == '|')
		token->type = FT_PIPE;
	else if (str[0] == '(')
		token->type = FT_OPEN_PAREN;
	else if (str[0] == ')')
		token->type = FT_CLOSE_PAREN;
	else if (str[0] == '>')
		token->type = FT_REDIR_OUT;
	else if (str[0] == '<')
		token->type = FT_REDIR_IN;
	else
		return (stt_token_word_handler(token, str));
	return (1 + ((token->type & (FT_APPND | FT_HRDOC | FT_AND | FT_OR)) != 0));
}

static
char	*stt_get_next_token(t_token *token, char *str)
{
	char	*ostr;

	ostr = str;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == 0 || *str == '#')
	{
		token->ptr = ostr;
		token->type = FT_END;
		token->length = (size_t)(str - ostr);
		return (str);
	}
	token->ptr = str;
	token->type = FT_UNSET;
	token->length = stt_token_finder(token, str);
	if (token->length == 0)
	{
		token->type = FT_ERROR;
		write(2, "minishell: syntax error near not implemented token\n", 51);
		return (NULL);
	}
	return (str + token->length);
}

int	tokenize(t_shell *shell, char *input)
{
	size_t	i;

	i = 0;
	while (true)
	{
		if (i >= FT_TOKEN_MAX - 1)
		{
			write(2, "too many tokens\n", 16);// ERROR_MSG
			shell->tokens[FT_TOKEN_MAX - 1].type = FT_END;
			return (-1);
		}
		input = stt_get_next_token(&shell->tokens[i], input);
		if (shell->tokens[i].type == FT_END)
			break ;
		if (shell->tokens[i].type == FT_ERROR)
			return (-1);
		i++;
	}
	if (i && syntax_validation(shell, 0) == -1)
		return (-1);
	return (i);
}
