/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/23 17:53:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_utils.h"

static int	stt_count_pipes(t_token *tokens, t_token *end)
{
	size_t	pipes_num;

	pipes_num = 0;
	while (tokens < end)
	{
		if (tokens[0].type & E_PIPE)
			pipes_num++;
		tokens++;
	}
	return (pipes_num);
}

static
int	stt_syntax_check(uint32_t cur_type, uint32_t prev_type, int *pdepth)
{
	if (cur_type & (E_OPAREN))
	{
		if (prev_type & (E_REDIR | E_WORD | E_CPAREN))
			return (-1);
		(*pdepth)++;
	}
	else if (cur_type & (E_CPAREN))
	{
		if (prev_type & (E_OPERATOR | E_REDIR | E_OPAREN))
			return (-1);
		(*pdepth)--;
	}
	if (*pdepth < 0)
		return (-1);
	if ((cur_type & E_OPERATOR)
		&& (prev_type & (E_OPERATOR | E_REDIR | E_OPAREN)))
		return (-1);
	if ((cur_type & E_WORD) && (prev_type & E_CPAREN))
		return (-1);
	if ((cur_type & E_REDIR) && (prev_type & E_REDIR))
		return (-1);
	return (0);
}

size_t	syntax_validation(t_token *tokens, t_token *end)
{
	int		pdepth;
	t_token	*start;

	start = tokens;
	if (tokens[0].type & (E_OPERATOR | E_CPAREN))
		return (ft_error("msh_syntax: ", "bad first token", 2), SIZE_MAX);
	pdepth = (tokens[0].type == E_OPAREN);
	while (!(tokens[1].type & (E_END)))
	{
		if (stt_syntax_check(tokens[1].type, tokens[0].type, &pdepth))
			return (ft_error("msh_syntax: ", "unexpected token", 2), SIZE_MAX);
		tokens++;
	}
	if (tokens[0].type & (E_OPERATOR | E_REDIR | E_OPAREN))
		return (ft_error("msh_syntax: ", "bad last token", 2), SIZE_MAX);
	if (pdepth > 0)
		return (ft_error("msh_syntax: ", "unclosed parenthesis", 2), SIZE_MAX);
	if (stt_count_pipes(start, end) >= FT_MAX_CHILDREN)
		return (ft_error("msh_syntax: ", "too many processes", 2), SIZE_MAX);
	return (0);
}
