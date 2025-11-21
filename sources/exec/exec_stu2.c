/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stu2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:14:41 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 23:18:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include "msh_utils.h"
#include <stdlib.h>
#include <signal.h>

// Need to know if it had a parenthesis inside
static
t_token	*stt_next_pipe(t_token *start, t_token *end, t_env *env, t_token **par_ptr)
{
	ssize_t		pdepth;
	uint32_t	type;
	t_token		*tokens;

	pdepth = 0;
	tokens = start;
	*par_ptr = NULL;
	while (tokens < end && !(tokens->type & E_END))
	{
		type = tokens->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
		if (*par_ptr == NULL && pdepth != 0)
			*par_ptr = tokens;
		if (pdepth == 0 && (type & E_PIPE))
			return (tokens);
		if ((type & E_REDIR) && pdepth == 0 && msh_apply_redir(tokens, env) < 0)
			return (NULL);
		if (pdepth < 0)
			ft_error("Houston we have a problem", "", -1);
		tokens++;
	}
	return (NULL);
}

int	exec_stu(t_token *start, t_token *end, t_env *env)
{
	int		rvalue;
	t_token	*next;
	t_token	*current;
	t_token	*par_ptr;
	pid_t	child_pids[32];	// Get from exec_ function returns

	if (start == end)
		return (0);
	current = start;
	next = stt_next_pipe(current, end, env, &par_ptr);
	while (current < end)
	{
		if (par_ptr == NULL)
		{
			// check if simple cmd: (if next == NULL and par_ptr == NULL)
			// yes) call exec_simple_cmd
			// no) fork, dup and exec_pipe
		}
		else
		{
			// fork, dup, call exec_subshell
		}
		if (next == NULL)
			break ;
		current = next + 1;
		next = stt_next_pipe(current, end, env, &par_ptr);
	}
	// Wait for children to finish
	// Close all FDs
	return (0);
}

// All of this in a loop where the next range is an Operator
// Need to open everything up until E_AND, E_OR, E_PIPE is found
// When a () is found, find its closing pair ignoring everything else
// Then find the next interval until a pipe or E_AND, E_OR, E_PIPE is found
// end is important for subshells

// 1) Open files, 2) Determine if we are calling a subshell
//	A) Subshell, B) Not Subshell
// If we are subshell, we call exec_subshell, which prepares stuff and calls exec_line again
// Which eventually will call exec_stu and return to the decision point
// If not subshell, proceed as regular