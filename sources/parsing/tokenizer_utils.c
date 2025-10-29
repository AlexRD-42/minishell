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

void	token_word_handler(t_token *token, char **input)
{
	const char	*start = *input;
	const char	*end;
	const char	*lookup_table = "\t\n\v\f\r |&();<>\"'";

	end = ft_strfind(start, lookup_table, 1);
	if (!end)
		end = start + ft_strlen(start);
	token->str = start;
	token->length = (size_t)(end - start);
	token->type = WORD;
	(*input) += token->length;
}

void	token_quote_handler(t_token *token, char **input)
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
		write(2, "minishell: syntax error: unclosed quote\n", 40);
		return ;
	}
	token->str = start;
	token->length = (size_t)(*input - start);
	if (quote == '"')
		token->type = DOUBLE_QUOTE;
	else
		token->type = SINGLE_QUOTE;
	(*input)++;
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
