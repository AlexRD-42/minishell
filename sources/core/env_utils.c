/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:11:30 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 18:26:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_utils.h"

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

// If length is 0, it will be calculated
char	*env_find(const char *entry, size_t length, size_t *index, t_env *env)
{
	size_t	i;
	size_t	j;

	if (length == 0)
		while (ft_ascii(entry[length]) & E_IDENT)
			length++;
	i = env->count * (length == 0);
	while (i < env->count)
	{
		j = 0;
		while (j < length && env->ptr[i][j] == entry[j])
			j++;
		if (j == length && env->ptr[i][j] == '=')
		{
			if (index != NULL)
				*index = i;
			return (env->ptr[i]);
		}
		i++;
	}
	if (index != NULL)
		*index = i;
	return (NULL);
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
	env->ptr[env->count] = NULL;	// I think this might be useless
	return (blocks_freed);
}

// Assumes entry name is SHLVL=
// -1) OOM
int	env_add_shlvl(t_env *env)
{
	char	buffer[64];
	char	*ptr;
	char	*env_entry;
	size_t	index;
	int64_t	level;

	env_entry = env_find("SHLVL", 5, &index, env);
	if (env_entry == NULL)
		return (env_add("SHLVL=1", env));
	level = ft_atoi(env_entry + 6) + 1;
	if (level <= 0 || level >= 1000)
		level = 1;
	ptr = ft_itoa_stack(level, buffer + sizeof(buffer) - 1);
	ptr -= 6;
	ft_memcpy(ptr, "SHLVL=", 6);
	return (env_replace(ptr, index, env));
}
