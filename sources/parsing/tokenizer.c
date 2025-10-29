/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/10/28 11:24:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Can be LUT
static void	\
find_token_type(t_token *token, char **input)
{
	if (cmp_token(input, "||"))
		token->type = OR;
	else if (cmp_token(input, "|"))
		token->type = PIPE;
	else if (cmp_token(input, "&&"))
		token->type = AND;
	else if (not_implemented(input))
		token->type = ERROR;
	else if (cmp_token(input, "("))
		token->type = OPEN_PARENTHESIS;
	else if (cmp_token(input, ")"))
		token->type = CLOSE_PARENTHESIS;
	else if (cmp_token(input, ">>"))
		token->type = APPEND;
	else if (cmp_token(input, "<<"))
		token->type = HEREDOC;
	else if (cmp_token(input, ">"))
		token->type = REDIRECT_OUT;
	else if (cmp_token(input, "<"))
		token->type = REDIRECT_IN;
	else if (**input == '\'' || **input == '"')
		handle_quote_token(token, input);
	else
		handle_word_token(token, input);
}

static void	\
get_next_token(t_token *token, char **input)
{
	while (ft_isspace(**input))
		(*input)++;
	if (!*input || **input == '\0')
	{
		token->type = END;
		return ;
	}
	token->str.kptr = *input;
	find_token_type(token, input);
	token->str.length = (size_t)(*input - token->str.kptr);
}

int	tokenize(t_shell *shell, char *input)
{
	int	i;

	i = 0;
	while (true)
	{
		if (i >= FT_TOKEN_MAX - 1)
		{
			write(2, "too many tokens\n", 16);
			shell->tokens[FT_TOKEN_MAX - 1].type = END;
			return (-1);
		}
		get_next_token(&shell->tokens[i], &input);
		if (shell->tokens[i].type == END)
			break ;
		if (shell->tokens[i].type == ERROR)
			return (-1);
		i++;
	}
	if (i && validate_syntax(shell, 0) == -1)
		return (-1);
	return (i);
}
