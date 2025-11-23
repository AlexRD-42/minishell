/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 12:15:52 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 16:30:50 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

void	msh_wait_child(pid_t *cpid_list, size_t count, t_env *env)
{
	size_t	i;
	int		status;

	i = 0;
	while (i < count)
	{
		waitpid(cpid_list[i], &status, 0);
		if (i == count - 1)
		{
			if (WIFEXITED(status))
				env->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				env->exit_status = 128 + WTERMSIG(status);
		}
		i++;
	}
}

t_token	*msh_next_delimiter(t_token *start, t_token *end, uint32_t delimiter)
{
	ssize_t		pdepth;
	uint32_t	type;

	pdepth = 0;
	while (start < end && !(start->type & E_END))
	{
		type = start->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
		if (pdepth == 0 && (type & delimiter))
			return (start);
		if (pdepth < 0)
		{
			ft_error("msh_unexpected: Negative parenthesis depth", "", 1);
			return (NULL);
		}
		start++;
	}
	return (NULL);
}

// To do: Check if expand_token null terminates the argument (it should)
// To do: Print error messages for exceeded count
ssize_t	msh_build_argv(t_token *token, t_env *env, t_vecp *argv)
{
	ssize_t	rvalue;

	while ((token->type & E_CMD_END) == 0 && argv->count < argv->max_count - 1)
	{
		if (token->type & E_WORD)
		{
			rvalue = expand_token(*token, env, argv);
			if (rvalue < 0)
			{
				if (rvalue == -4)
					ft_error("msh_memory: argv out of bounds", "", -1);
				return (rvalue);
			}
		}
		token++;
	}
	argv->ptr[argv->count] = NULL;
	return ((ssize_t) argv->count);
}

// Always exits upon return
int	exec_subshell(t_token *start, t_env *env)
{
	t_token		*end;
	size_t		pdepth;
	uint32_t	type;

	start += !!(start->type & E_OPEN_PAREN);	// Could just do start++
	end = start;
	type = end->type;
	pdepth = 1 - !!(type & E_CLOSE_PAREN);
	while (pdepth != 0)		// Dangerous if pre-validation is wrong but good for debugging
	{
		end++;
		type = end->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
	}
	_exit (exec_line(start, end - 1, env));	// Double check the math here
}

// Returns 1 if a command was executed
int exec_simple(t_token *start, t_token *end, t_env *env)
{
	const int32_t	prev_fd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	t_vecp  		argv;
	char			*arg_ptr[FT_ARG_COUNT];
	char			buf[FT_ARG_MAX];			// Kernel FT_ARG_MAX is 2 MB
	int	 			rvalue;

	if (prev_fd[0] < 0 || prev_fd[1] < 0)
		return (ft_error("msh_dup: ", NULL, -1));	// Failed to save the state
	if (msh_mutates_state(start, end) == 0)
		return (0);	// Not simple command
	argv = (t_vecp){{buf, buf + sizeof(buf), buf}, 0, FT_ARG_COUNT, arg_ptr};
	if (msh_build_argv(start, env, &argv) <= 0 || !argv.ptr[0])
		_exit(1); // FELIPE: 103 e 105 were "return (-1)" but were creating zombie child
	rvalue = msh_dispatch(&argv, env);	// Review: Do we care about the rvalue here?
	if ((dup2(prev_fd[0], STDIN_FILENO) < 0) + (dup2(prev_fd[1], STDOUT_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);	// Restores fds
	close(prev_fd[0]);	// Do we throw error messages for close?
	close(prev_fd[1]);
	return (1);		// It was simple command and it executed
}
