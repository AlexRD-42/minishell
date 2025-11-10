/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 12:36:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include "minishell.h"
#include "msh_types.h"

// Variable name is defined by all letters until not alphanumeric
// This name is used to find the var name in ENV
// Updates str and buffer to the end of their respective copy
void	expand_word(const char **str, char **buffer, t_env *env)
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
		return ;
	ptr = env->ptr[index];
	while (*ptr != 0 && *ptr != '=')
		ptr++;
	length = 0;
	while (ptr[length] != 0)
		length++;
	ft_memcpy(*buffer, ptr, length);
	*buffer += length;
}

// cat f*n*e (needs fuzzy matching)
void	expand_dir(const char **str, char **buffer, t_env *env)
{
	
}
