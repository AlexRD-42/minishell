/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:11:30 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 12:04:51 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "minishell.h"

size_t	mark_for_deletion(char *ptr, t_env *env)
{
	size_t			start;
	size_t			meta_index;
	char			*metadata;
	const size_t	max_count = env->max_count;

	metadata = env->metadata;
	meta_index = (size_t)(ptr - env->optr) / FT_BLOCK_SIZE;
	start = meta_index;
	metadata[meta_index++] = E_META_FREE;
	while (meta_index < max_count && metadata[meta_index] >= E_META_USED)
	{
		metadata[meta_index] = E_META_FREE;
		meta_index++;
	}
	return (meta_index - start);	// Blocks freed
}

size_t	env_del(const char *entry, t_env *env)
{
	char	*ptr;
	size_t	index;
	size_t	blocks_freed;

	ptr = env_find(entry, 0, &index, env);
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
