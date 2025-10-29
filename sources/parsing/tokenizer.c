/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/07/13 11:18:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	\
stt_token_cmp(char **input, char *token)
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

static void	\
stt_token_finder(t_token *token, char **input)
{
	if (stt_token_cmp(input, "||"))
		token->type = OR;
	else if (stt_token_cmp(input, "|"))
		token->type = PIPE;
	else if (stt_token_cmp(input, "&&"))
		token->type = AND;
	else if (token_not_implemented(input))
		token->type = ERROR;
	else if (stt_token_cmp(input, "("))
		token->type = OPEN_PAREN;
	else if (stt_token_cmp(input, ")"))
		token->type = CLOSE_PAREN;
	else if (stt_token_cmp(input, ">>"))
		token->type = APPEND;
	else if (stt_token_cmp(input, "<<"))
		token->type = HEREDOC;
	else if (stt_token_cmp(input, ">"))
		token->type = REDIRECT_OUT;
	else if (stt_token_cmp(input, "<"))
		token->type = REDIRECT_IN;
	else if (**input == '\'' || **input == '"')
		token_quote_handler(token, input);
	else
		token_word_handler(token, input);
}

static inline int	\
stt_ft_isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

static void	\
stt_get_next_token(t_token *token, char **input)
{
	while (stt_ft_isspace(**input))
		(*input)++;
	if (!*input || **input == '\0')
	{
		token->type = END;
		return ;
	}
	token->str = *input;
	stt_token_finder(token, input);
	token->length = (size_t)(*input - token->str);
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
		stt_get_next_token(&shell->tokens[i], &input);
		if (shell->tokens[i].type == END)
			break ;
		if (shell->tokens[i].type == ERROR)
			return (-1);
		i++;
	}
	if (i && syntax_validation(shell, 0) == -1)
		return (-1);
	return (i);
}
