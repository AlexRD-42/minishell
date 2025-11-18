/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:11:30 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 18:44:53 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "minishell.h"
#include "msh_test.h"

size_t	mark_for_deletion(char *ptr, t_env *env)
{
	size_t	start;
	size_t	meta_index;
	char	*metadata;

	metadata = env->metadata;
	meta_index = (size_t)(ptr - env->optr) / BLOCK_SIZE;
	start = meta_index;
	metadata[meta_index++] = 0;
	while (meta_index < env->count && meta_index >= 2)
	{
		metadata[meta_index] = 0;
		meta_index++;
	}
	return (meta_index - start);	// Blocks freed
}

// val, val1, val2, null
size_t	envx_del(t_env *env, const char *entry)
{
	char	*ptr;
	size_t	index;
	size_t	blocks_freed;

	ptr = envx_find(env, entry, SIZE_MAX, &index);
	if (ptr == NULL)
		return (SIZE_MAX);
	blocks_freed = mark_for_deletion(ptr, env);
	while (index < env->count)
	{
		env->ptr[index] = env->ptr[index + 1];
		index++;
	}
	env->count--;
	return (blocks_freed);
}
