/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_dispatch.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:29:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 12:21:16 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"

// echo with option -n
// cd with only a relative or absolute path
// pwd with no options
// export with no options
// unset with no options
// env with no options or arguments
// exit with no options

static inline
uint_fast8_t	stt_is_space(const char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static inline
uint_fast8_t	stt_end(const char c)
{
	return (c == 0 || c == ' ' || (c >= 9 && c <= 13));
}

// Need to check the proper returns
// -1 is impossible, so if the return is < 0 the builtin wasnt found
int msh_dispatch(t_vecp *argv, t_env *env)
{
	const char *str = argv->ptr[0];
	
	if (str[0] == 'e')
	{
		if (str[1] == 'x' && str[2] == 'i' && str[3] == 't' && stt_end(str[4]))
			return (msh_exit());
		if (str[1] == 'x' && str[2] == 'p' && str[3] == 'o' && str[4] == 'r'
			&& str[5] == 't' && stt_end(str[6]))
			return (msh_export(argv, env));
		if (str[1] == 'n' && str[2] == 'v' && stt_end(str[3]))
			return (msh_env(env));
		if (str[1] == 'c' && str[2] == 'h' && str[3] == 'o' && stt_end(str[4]))
			return (msh_echo(argv));
		return (-1);
	}
	if (str[0] == 'c' && str[1] == 'd' && stt_end(str[2]))
		return (msh_cd(env));
	if (str[0] == 'p' && str[1] == 'w' && str[2] == 'd' && stt_end(str[3]))
		return (msh_pwd());
	if (str[0] == 'u' && str[1] == 'n' && str[2] == 's' && str[3] == 'e'
		&& str[4] == 't' && stt_end(str[5]))
		return (msh_unset(argv, env));
	return (-1);
}
