/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/07/13 11:18:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_check_start(t_token first_token)
{
	if (first_token.type & (OPERATOR | CLOSE_PAREN))
		return (syntax_print_error(first_token), 1);
	return (0);
}

int	syntax_check_end(t_token last_token)
{
	if (last_token.type & (OPERATOR | REDIR | OPEN_PAREN))
		return (write(2,
				"mini: syntax error near unexpected token 'newline'\n", 51),
			1);
	return (0);
}

void	syntax_print_error(t_token token)
{
	write(2, "mini: syntax error near unexpected token '", 42);
	write(2, token.ptr, token.length);
	write(2, "'\n", 2);
}
