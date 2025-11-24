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

static int	stt_get_max_forks(t_token *start, t_token *end)
{
	int		current_max;
	int		overall_max;
    t_token	*delimiter;
	t_token	*current_start;

	if (start >= end)
		return (0);
	overall_max = 0;
	current_start = start;
	while (current_start < end)
	{
		delimiter = msh_next_delimiter(current_start, end, E_PIPE | E_OR);
		current_max = stt_count_pipeline_forks(current_start, delimiter, 0);
		if (current_max > overall_max)
			overall_max = current_max;
		current_start = delimiter + 1;
	}
	return (overall_max);
}

static int	
stt_count_pipeline_forks(t_token *start, t_token *end, int max_forks)
{
	int		pipe_count;
	int		internal_max;
	t_token	*close;

	pipe_count = 0;
	while (start < end)
	{
		if (start->type & E_PIPE)
			pipe_count++;
		else if (start->type & E_OPAREN)
		{
			close = msh_next_delimiter(start + 1, end, E_CPAREN);
			internal_max = stt_get_max_forks(start + 1, close); 
			if (internal_max > max_forks)
				max_forks = internal_max;
			pipe_count++; 
			start = close;
		}
		else if (pipe_count == 0 && start->type & E_WORD)
			pipe_count = 1;
		if (pipe_count > max_forks)
			max_forks = pipe_count;
		start++;
	}
	return (max_forks);
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
	if ((cur_type & E_OPERATOR) && (prev_type & (E_OPERATOR | E_REDIR | E_OPAREN)))
		return (-1);	// operator
	if ((cur_type & E_WORD) && (prev_type & E_CPAREN))
		return (-1);	// word
	if ((cur_type & E_REDIR) && (prev_type & E_REDIR))
		return (-1);	// redirect
	return (0);
}

size_t	syntax_validation(t_token *tokens, t_token *end)
{
	int		pdepth;
	t_token	*start;

	start = tokens;
	if (tokens[0].type & (E_OPERATOR | E_CPAREN))
		return (SIZE_MAX);	// Print error first token bad
	pdepth = (tokens[0].type == E_OPAREN);
	while (!(tokens[1].type & (E_END)))
	{
		if (stt_syntax_check(tokens[1].type, tokens[0].type, &pdepth))
			return (SIZE_MAX);	// Print error at token ptr
		tokens++;
	}
	if (tokens[0].type & (E_OPERATOR | E_REDIR | E_OPAREN))
		return (SIZE_MAX);	// Print error 
	if (pdepth > 0)
		return (SIZE_MAX);	// Unclosed (
	if (stt_get_max_forks(start, end) >= FT_MAX_CHILDREN)
		return (SIZE_MAX); // MAX CHILDREN
	return (0);
}
