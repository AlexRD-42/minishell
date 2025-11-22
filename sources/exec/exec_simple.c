/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 17:53:48 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 19:38:01 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

static inline
uint8_t	stt_end(const char c)
{
	return (c == 0 || c == ' ' || (c >= 9 && c <= 13));
}

bool	stt_is_builtin(t_token *start, t_token *end)
{
	const char	*str;
	bool		rvalue;

	while (start < end && !(start->type & (E_WORD | E_PIPE)))
		start++;
	str = start->ptr;
	rvalue = (str[0] == 'e' && 
		((str[1] == 'x' && str[2] == 'i' && str[3] == 't' && stt_end(str[4]))
		|| (str[1] == 'x' && str[2] == 'p' && str[3] == 'o' && str[4] == 'r' && str[5] == 't' && stt_end(str[6]))))
		|| (str[0] == 'c' && str[1] == 'd' && stt_end(str[2]))
		|| (str[0] == 'u' && str[1] == 'n' && str[2] == 's' && str[3] == 'e' && str[4] == 't' && stt_end(str[5]));
	return (rvalue);
}

// Returns 1 if a command was executed
int exec_simple(t_token *start, t_token *end, t_env *env)
{
	const int32_t	prev_fd[2] = {dup(STDIN_FILENO), dup(STDOUT_FILENO)};
	char			*arg_ptr[FT_ARG_COUNT];
	char			buffer[FT_ARG_SIZE];
	t_vecp  		argv;
	int	 			rvalue;

	if (prev_fd[0] < 0 || prev_fd[1] < 0)
		return (ft_error("msh_dup: ", NULL, -1));	// Failed to save the state
	if (stt_is_builtin(start, end) == 0)
		return (0);	// Not simple command
	argv = (t_vecp){{buffer, buffer + sizeof(buffer), buffer}, 0, FT_ARG_COUNT, arg_ptr};
	if (msh_build_argv(start, env, &argv) < 0)
		return (-1);
	rvalue = msh_dispatch(&argv, env);	// Do we care about the rvalue here?
	if ((dup2(prev_fd[0], STDIN_FILENO) < 0) + (dup2(prev_fd[1], STDOUT_FILENO) < 0))
		ft_error("msh_dup2: ", NULL, -1);	// Restores fds
	close(prev_fd[0]);	// Do we throw error messages for close?
	close(prev_fd[1]);
	return (1);		// It was simple command and it executed
}
