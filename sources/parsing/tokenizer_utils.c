/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/10/28 12:34:59 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

// Podemos usar ft_strfind
static inline int	\
stt_findset(char *set, int c)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if ((int)set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	cmp_token(char **input, char *token)
{
	size_t	token_len;

	token_len = 1;
	if (token[1])
		token_len++;
	if (ft_strncmp(*input, token, token_len) == 0)
	{
		(*input) += token_len;
		return (1);
	}
	return (0);
}

void	handle_word_token(t_token *token, char **input)
{
	const char	*start = *input;

	while (**input && !ft_isspace(**input)
		&& !stt_findset("|&();<>\n\"'", **input))
		(*input)++;
	token->str.kptr = start;
	token->str.length = (size_t)(*input - start);
	token->type = WORD;
}

void	handle_quote_token(t_token *token, char **input)
{
	const char	*start = *input;
	char		quote;

	quote = **input;
	(*input)++;
	while (**input && **input != quote)
		(*input)++;
	if (**input != quote)
	{
		token->type = ERROR;
		write(2, "minishell: syntax error: unclosed quote\n", 41);
		return ;
	}
	token->str.kptr = start;
	token->str.length = (size_t)(*input - start);
	if (quote == '"')
		token->type = DOUBLE_QUOTE;
	else
		token->type = SINGLE_QUOTE;
	(*input)++;
}
