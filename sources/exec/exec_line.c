/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:34:39 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/23 13:45:15 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

static
int stt_wait_child(pid_t *cpid_list, size_t count)
{
	size_t	i;
	int		status;
	int		last_status;

	i = 0;
	last_status = 0;
	while (i < count)
	{
		waitpid(cpid_list[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status);
}

// Receives {start, start, start, end}
int	exec_stu(t_token_range *token, t_env *env)
{
	const int32_t	original_stdin = dup(STDIN_FILENO);
	size_t			count;
	pid_t			cpid_list[32];
	pid_t			process_id;
	int				rvalue;

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
	rvalue = stt_wait_child(cpid_list, count);
	if ((dup2(original_stdin, STDIN_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);
	close(original_stdin);
	return (rvalue);
}

static
t_token	*stt_next_stu(t_token *start, t_token *end)
{
	ssize_t		pdepth;
	uint32_t	type;
	t_token		*tokens;

	pdepth = 0;
	tokens = start;
	while (tokens < end && (tokens->type & (E_END)) == 0)
	{
		type = tokens->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
		if (pdepth == 0 && (type & (E_AND | E_OR)))
			return (tokens);
		else if (pdepth < 0)
		{
			ft_error("Houston we have a problem", "", -1);
			return (NULL);
		}
		tokens++;
	}
	return (NULL);
}

int	exec_line(t_token *start, t_token *end, t_env *env)
{
	int		status;
	t_token	*next;
	t_token	*current;

	if (start == end)
		return (0);
	current = start;
	next = stt_next_stu(current, end);
	while (next != NULL)
	{
		status = exec_stu(&(t_token_range){current, current, current, end}, env);
		if ((next->type & E_AND) && status != 0)
			return (status);
		if ((next->type & E_OR) && status == 0)
			return (status);
		current = next + 1;
		next = stt_next_stu(current, end);
	}
	return (exec_stu(&(t_token_range){current, current, current, end}, env));
}
