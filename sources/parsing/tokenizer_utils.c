/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/07/13 11:18:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static int	\
stt_token_quote_handler(t_token *token, char **end, char quote)
{
	(*end)++;
	while (**end && **end != quote)
		(*end)++;
	if (!**end)
	{
		token->type = ERROR;
		write(2, "minishell: syntax error: unclosed quote\n", 40);
		return (1);
	}
	(*end)++;
	return (0);
}

// ft_strfind but working with (char *)s instead of (const char *)s 
static char	\
*stt_ft_strfind(char *str, char *charset, uint8_t ref)
{
	size_t	i;
	size_t	lookup_table[256 / sizeof(size_t)];

	i = 0;
	while (i < (256 / sizeof(size_t)))
		lookup_table[i++] = 0UL;
	while (*charset != 0)
	{
		((uint8_t *)lookup_table)[(uint8_t)(*charset)] = 1;
		charset++;
	}
	ref = (ref != 0);
	((uint8_t *)lookup_table)[0] = ref;
	while (((uint8_t *)lookup_table)[(uint8_t)(*str)] != ref)
		str++;
	if (*str != 0)
		return (str);
	return (NULL);
}

void	token_word_handler(t_token *token, char **input)
{
	char	*start;
	char	*lookup_table;
	char	*end;
	char	quote;

	start = *input;
	lookup_table = "\t\n\v\f\r |&();<>'\"";
	end = stt_ft_strfind(start, lookup_table, 1);
	if (!end)
		end = start + ft_strlen(start);
	while (*end == '"' || *end == '\'')
	{
		quote = *end;
		if (stt_token_quote_handler(token, &end, quote))
			return ;
		end = stt_ft_strfind(end, lookup_table, 1);
		if (!end)
			break ;
	}
	if (!end)
		end = start + ft_strlen(start);
	token->str = start;
	token->length = (size_t)(end - start);
	token->type = WORD;
	(*input) += token->length;
}

int	token_not_implemented(char **input)
{
	if (!ft_strncmp(*input, "&", 1))
	{
		write(2, "minishell: syntax error near unexpected token '&'\n", 50);
		return (-1);
	}
	if (!ft_strncmp(*input, ";", 1))
	{
		write(2, "minishell: syntax error near unexpected token ';'\n", 50);
		return (-1);
	}
	if (!ft_strncmp(*input, ">>>", 3))
	{
		write(2, "minishell: syntax error near unexpected token '>'\n", 50);
		return (-1);
	}
	if (!ft_strncmp(*input, "<<<", 3))
	{
		write(2, "minishell: syntax error near unexpected token '<'\n", 50);
		return (-1);
	}
	return (0);
}
