/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 01:25:38 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "msh_types.h"
#include "msh_defines.h"
#include "msh_utils.h"

static
char	*stt_assign_blocks(t_env *env, size_t meta_index, size_t lower, size_t upper)
{
	size_t	i;
	char	*metadata;

	metadata = env->metadata;
	i = meta_index;
	lower += i;
	upper += i;
	metadata[i++] = E_META_HEAD;
	while (i < lower)
	{
		metadata[i] = E_META_USED;
		i++;
	}
	while (i < upper)
	{
		metadata[i] = E_META_RESERVED;
		i++;
	}
	return (env->optr + meta_index * FT_BLOCK_SIZE);
}

static
size_t	stt_find_space(char *metadata, size_t max_count, size_t lower, size_t *out)
{
	size_t	i;
	size_t	block_index;
	size_t	upper;

	upper = 4 * lower;
	*out = 0;
	while (upper >= lower)
	{
		i = 0;
		while (i < max_count)
		{
			block_index = i + upper;	// use ft_max
			while (i < max_count && metadata[i] == 0 && i < block_index)
				i++;
			if (i == block_index)
			{
				*out = upper;
				return (block_index - upper);
			}
			i++;
		}
		upper >>= 1;
	}
	return (SIZE_MAX);
}

static
void	stt_update_ptrs(char *optr, char **envp, size_t count)
{
	size_t	i;
	char	*wptr;

	i = 0;
	wptr = optr;
	while (i < count)
	{
		envp[i++] = wptr;
		wptr += (1 + (ft_strlen(wptr) + 1) / FT_BLOCK_SIZE) * FT_BLOCK_SIZE;
	}
	envp[i] = NULL;	
}

static
void	stt_compact(t_env *env)
{
	size_t	i;
	size_t	length;
	size_t	num_blocks;
	size_t	meta_index;
	char	buffer[FT_ENV_SIZE];

	i = 0;
	meta_index = 0;
	ft_memset(env->metadata, 0, FT_ENV_COUNT);
	while (i < env->count)
	{
		length = ft_strlen(env->ptr[i]) + 1;
		ft_memcpy(buffer + meta_index * FT_BLOCK_SIZE, env->ptr[i], length);
		num_blocks = 1 + length / FT_BLOCK_SIZE;
		stt_assign_blocks(env, meta_index, num_blocks, num_blocks);
		meta_index += num_blocks;
		i++;
	}
	ft_memcpy(env->optr, buffer, meta_index * FT_BLOCK_SIZE);
	stt_update_ptrs(env->optr, env->ptr, env->count);
}

// Assign blocks, calls free if necessary
char	*allocate_blocks(t_env *env, size_t bytes)
{
	size_t			meta_index;
	const size_t	lower = 1 + bytes / FT_BLOCK_SIZE;
	size_t			upper;

	meta_index = stt_find_space(env->metadata, env->max_count, lower, &upper);
	if (meta_index == SIZE_MAX)
	{
		stt_compact(env);
		meta_index = stt_find_space(env->metadata, env->max_count, lower, &upper);
		if (meta_index == SIZE_MAX)
			return (NULL);
	}
	return (stt_assign_blocks(env, meta_index, lower, upper));
}
