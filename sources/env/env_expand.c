/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:40:45 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/13 13:12:51 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include "minishell.h"

// Variable name is defined by all letters until not alphanumeric
// This name is used to find the var name in ENV
// Updates str and buffer to the end of their respective copy
// To do: Create an env helper that returns the value rather than the entry
// To do: Fix $ length = 0, add $?
// Return: 0) OK, 1) OOM
uint8_t	env_expand(const char **str, t_argv *arg, t_env *env)
{
	size_t		index;
	size_t		length;
	const char	*ptr = *str;

	length = 0;
	while (ft_ascii(ptr[length]) & E_IDENT)
		length++;
	index = env_find(env, *str, length);
	*str += length;
	if (index == SIZE_MAX)
		return (0);	// Check
	ptr = env->ptr[index];
	while (*ptr != 0 && *ptr != '=')
		ptr++;
	ptr += *ptr == '=';
	length = 0;
	while (ptr[length] != 0)
		length++;
	if (ft_lmcpy(arg->data + arg->offset, ptr, length, arg->end))
		return (1);
	arg->offset += length;
	return (0);
}
