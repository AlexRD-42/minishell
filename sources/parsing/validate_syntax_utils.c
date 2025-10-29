/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/10/28 11:35:03 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	start_error(t_token first_token)
{
	if (first_token.type & (OR
			| AND
			| CLOSE_PARENTHESIS
			| PIPE
			| REDIRECT_OUT
			| APPEND
			| HEREDOC))
		return (syntax_error(first_token), -1);
	return (0);
}

int	end_error(t_token last_token)
{
	if (last_token.type & (OR
			| AND
			| PIPE
			| REDIRECT_IN
			| REDIRECT_OUT
			| APPEND
			| HEREDOC
			| OPEN_PARENTHESIS))
		return (write(2,
				"mini: syntax error near unexpected token 'newline'\n", 57),
			-1);
	return (0);
}

int	ft_isspace(int c)
{
	// return (c == '\t' || c == ' ' || c == '\n' || c == '\f' || c == '\r'
	// 	|| c == '\v');
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	not_implemented(char **input)
{
	if (!ft_strncmp(*input, "&", 1))
	{
		write(2, "minishell: syntax error near unexpected token '&'\n", 51);
		return (-1);
	}
	if (!ft_strncmp(*input, ";", 1))
	{
		write(2, "minishell: syntax error near unexpected token ';'\n", 51);
		return (-1);
	}
	if (!ft_strncmp(*input, ">>>", 3))
	{
		write(2, "minishell: syntax error near unexpected token '>'\n", 51);
		return (-1);
	}
	if (!ft_strncmp(*input, "<<<", 3))
	{
		write(2, "minishell: syntax error near unexpected token '>'\n", 51);
		return (-1);
	}
	return (0);
}

void	syntax_error(t_token token)
{
	write(2, "minishell: syntax error near unexpected token '", 48);
	write(2, token.str.kptr, token.str.length);
	write(2, "'\n", 2);
}
