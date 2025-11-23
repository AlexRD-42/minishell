/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/23 16:43:05 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

// Receives {start, start, start, end}
int	exec_stu(t_token_range *token, t_env *env)
{
	const int32_t	original_stdin = dup(STDIN_FILENO);
	size_t			count;
	pid_t			cpid_list[FT_MAX_CHILDREN];	// No overflow because it is prevalidated
	pid_t			process_id;

	if (token->start == token->end)
		return (0);
	if (original_stdin < 0)
		return (ft_error("msh_dup: ", NULL, -1));	// Failed to save the state
	count = 0;
	while (token->current < token->end)
	{
		process_id = exec_pipe(token, env);
		if (process_id > 0)	// Does not save ID of parent executions
			cpid_list[count++] = process_id;
		if (token->next == NULL)
			break ;
		token->current = token->next + 1;
	}
	msh_wait_child(cpid_list, count, env);
	if ((dup2(original_stdin, STDIN_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);
	close(original_stdin);
	return (0);
}

int	exec_line(t_token *start, t_token *end, t_env *env)
{
	t_token	*next;
	t_token	*current;

	if (start == end)
		return (0);
	current = start;
	next = msh_next_delimiter(current, end, E_AND | E_OR);
	while (next != NULL)
	{
		env->exit_status = exec_stu(&(t_token_range){current, current, current, end}, env);
		if ((next->type & E_AND) && env->exit_status != 0)
			return (env->exit_status);
		if ((next->type & E_OR) && env->exit_status == 0)
			return (env->exit_status);
		current = next + 1;
		next = msh_next_delimiter(current, end, E_AND | E_OR);
	}
	return (exec_stu(&(t_token_range){current, current, current, end}, env));
}
