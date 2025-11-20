/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:11:43 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 13:16:55 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_utils.h"

static
size_t	stt_get_blocks_used(t_env *env, const char *ptr)
{
	size_t			i;
	size_t			meta_index;
	char			*metadata;
	const size_t	count = env->max_count;

	metadata = env->metadata;
	meta_index = (size_t)(ptr - env->optr) / FT_BLOCK_SIZE;
	i = meta_index + (metadata[meta_index] == E_META_HEAD);
	while (i < count && metadata[i] >= E_META_USED)
		i++;
	return (i - meta_index);
}

static
int	stt_replace(t_str old_entry, t_kstr new_entry, size_t index, t_env *env)
{
	size_t	blocks_needed;
	size_t	blocks_used;
	char	*wptr;

	while (new_entry.ptr[new_entry.length] != 0)
		new_entry.length++;
	blocks_needed = 1 + (1 + new_entry.length / FT_BLOCK_SIZE);
	blocks_used = stt_get_blocks_used(env, old_entry.ptr);
	if (blocks_needed > blocks_used)
	{
		wptr = allocate_blocks(env, new_entry.length + 1);
		if (wptr == NULL)
			return (-1);
		mark_for_deletion(old_entry.ptr, env);	// Always after request for memcpy
		ft_memcpy(wptr, new_entry.ptr, new_entry.length + 1);
		env->ptr[index] = wptr;
	}
	else
		ft_memcpy(old_entry.ptr, new_entry.ptr, new_entry.length + 1);
	return (0);
}

// Adding substr to entry
// Check if substr received is past =
static
int	stt_append(t_str old_entry, t_kstr new_entry, size_t index, t_env *env)
{
	size_t	total_size;
	size_t	blocks_used;
	size_t	blocks_needed;
	char	*wptr;

	while (new_entry.ptr[new_entry.length] != 0)
		new_entry.length++;
	total_size = old_entry.length + new_entry.length + 1;
	blocks_used = stt_get_blocks_used(env, old_entry.ptr);
	blocks_needed = 1 + total_size / FT_BLOCK_SIZE;
	if (blocks_needed > blocks_used)
	{
		wptr = allocate_blocks(env, total_size);
		if (wptr == NULL)
			return (-1);	// OOM
		mark_for_deletion(old_entry.ptr, env);		// Prevents aliasing if its after
		old_entry.ptr = ft_memcpy(wptr, old_entry.ptr, old_entry.length);
		env->ptr[index] = old_entry.ptr;	// Reassigns pointer to new block
	}
	old_entry.ptr += old_entry.length;
	ft_memcpy(old_entry.ptr, new_entry.ptr, new_entry.length + 1);
	return (0);
}

static
int	stt_add(t_kstr new_entry, t_env *env)
{
	char	*wptr;

	while (new_entry.ptr[new_entry.length] != 0)
		new_entry.length++;
	wptr = allocate_blocks(env, new_entry.length + 1);
	if (wptr == NULL)
		return (-1);
	ft_memcpy(wptr, new_entry.ptr, new_entry.length + 1);
	env->ptr[env->count] = wptr;
	env->count++;
	return (0);
}

// New_entry string should contain the length of the variable name (when it is append)
// Index == SIZE_MAX means the variable does not exist in ENV
int	env_add(t_kstr new_entry, size_t index, bool append, t_env *env)
{
	t_str		old_entry;

	if (index == SIZE_MAX && append == 0)
		return (stt_add(new_entry, env));
	else if (index == SIZE_MAX)
		return (-1);
	old_entry.ptr = env->ptr[index];
	old_entry.length = ft_strlen(old_entry.ptr);
	if (append == 1)
	{
		new_entry.ptr += new_entry.length + (*new_entry.ptr == '+');
		new_entry.ptr += (*new_entry.ptr == '=');
		return (stt_append(old_entry, new_entry, index, env));
	}
	return (stt_replace(old_entry, new_entry, index, env));
}
