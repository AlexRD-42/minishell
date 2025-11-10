/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_dispatch.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:29:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/06 22:38:33 by adeimlin         ###   ########.fr       */
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

builtin_fn msh_dispatch(const char *str)
{
	while (stt_is_space(*str) == 1)
		str++;
	if (str[0] == 'e')
	{
		if (str[1] == 'x' && str[2] == 'i' && str[3] == 't' && stt_end(str[4]))
			return (msh_exit);
		if (str[1] == 'x' && str[2] == 'p' && str[3] == 'o' && str[4] == 'r'
			&& str[5] == 't' && stt_end(str[6]))
			return (msh_export);
		if (str[1] == 'n' && str[2] == 'v' && stt_end(str[3]))
			return (msh_env);
		if (str[1] == 'c' && str[2] == 'h' && str[3] == 'o' && stt_end(str[4]))
			return (msh_echo);
		return (NULL);
	}
	if (str[0] == 'c' && str[1] == 'd' && stt_end(str[2]))
		return (msh_cd);
	if (str[0] == 'p' && str[1] == 'w' && str[2] == 'd' && stt_end(str[3]))
		return (msh_pwd);
	if (str[0] == 'u' && str[1] == 'n' && str[2] == 's' && str[3] == 'e'
		&& str[4] == 't' && stt_end(str[5]))
		return (msh_unset);
	return (NULL);
}

int msh_dispatch2(const char *str)
{
	while (stt_is_space(*str) == 1)
		str++;
	if (str[0] == 'e')
	{
		if (str[1] == 'x' && str[2] == 'i' && str[3] == 't' && stt_end(str[4]))
			return (msh_exit());
		if (str[1] == 'x' && str[2] == 'p' && str[3] == 'o' && str[4] == 'r'
			&& str[5] == 't' && stt_end(str[6]))
			return (msh_export());
		if (str[1] == 'n' && str[2] == 'v' && stt_end(str[3]))
			return (msh_env());
		if (str[1] == 'c' && str[2] == 'h' && str[3] == 'o' && stt_end(str[4]))
			return (msh_echo());
		return (NULL);
	}
	if (str[0] == 'c' && str[1] == 'd' && stt_end(str[2]))
		return (msh_cd());
	if (str[0] == 'p' && str[1] == 'w' && str[2] == 'd' && stt_end(str[3]))
		return (msh_pwd());
	if (str[0] == 'u' && str[1] == 'n' && str[2] == 's' && str[3] == 'e'
		&& str[4] == 't' && stt_end(str[5]))
		return (msh_unset());
	return (127);	// Check!
}