/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:11:43 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 18:46:44 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_test.h"

static
size_t	stt_get_blocks_used(t_env *env, const char *ptr)
{
	size_t			i;
	size_t			meta_index;
	char			*metadata;
	const size_t	count = env->max_count;

	metadata = env->metadata;
	meta_index = (size_t)(ptr - env->optr) / BLOCK_SIZE;
	i = meta_index + (metadata[meta_index] == 1);
	while (i < count && metadata[i] >= 2)
		i++;
	return (i - meta_index);
}

// Adding substr to entry
// Free first with buffer would allow more leniency with space
int	envx_replace(t_str old_entry, t_kstr new_entry, size_t index, t_env *env)
{
	const size_t	blocks_needed = 1 + new_entry.length / BLOCK_SIZE;
	const size_t	blocks_used = stt_get_blocks_used(env, old_entry.ptr);
	char			*wptr;

	if (blocks_needed > blocks_used)
	{
		wptr = request_blocks(env, new_entry.length + 1);
		if (wptr == NULL)
			return (-1);
		mark_for_deletion(old_entry.ptr, env);
		ft_memcpy(wptr, new_entry.ptr, new_entry.length + 1);
		env->ptr[index] = wptr;
	}
	else
		ft_memcpy(old_entry.ptr, new_entry.ptr, new_entry.length + 1);
	return (0);
}

int	envx_add(t_kstr new_entry, t_env *env)
{
	char			*wptr;

	wptr = request_blocks(env, new_entry.length + 1);
	if (wptr == NULL)
		return (-1);
	ft_memcpy(wptr, new_entry.ptr, new_entry.length + 1);
	env->ptr[env->count] = wptr;
	env->count++;
	return (0);
}

// Adding substr to entry
// Free first with buffer would allow more leniency with space
int	envx_append(t_str str, t_kstr substr, size_t index, t_env *env)
{
	const size_t	total_length = str.length + substr.length + 1;
	const size_t	blocks_used = stt_get_blocks_used(env, str.ptr);
	const size_t	blocks_needed = 1 + total_length / BLOCK_SIZE;
	char			*wptr;

	if (blocks_needed > blocks_used)
	{
		wptr = request_blocks(env, total_length);
		if (wptr == NULL)
			return (-1);	// OOM
		mark_for_deletion(str.ptr, env);
		ft_memcpy(wptr, str.ptr, str.length);
		env->ptr[index] = wptr;	// Reassigns pointer to new block
	}
	ft_memcpy(str.ptr + str.length, substr.ptr, substr.length + 1);
	return (0);
}

// -1) OOM
// V lower V upper (length)
// variable=value
// 0: New Entry, 1: Overwrite, 2) Error, 3) Append
int	mshx_export(const char *entry, t_env *env)
{
	t_str		env_entry;
	size_t		name_length;
	size_t		var_length;
	size_t		index;
	size_t		type;

	name_length = 0;
	while (entry[name_length] != 0 && entry[name_length] != '=')
		name_length++;
	type = (name_length > 1) && (entry[name_length] == '=')	&& entry[name_length - 1] == '+';
	env_entry.ptr = envx_find(env, entry, name_length - type, &index);
	name_length += (entry[name_length] == '=');
	var_length = 0;
	while (entry[name_length + var_length] != 0)
		var_length++;
	type = (env_entry.ptr != NULL) + (type << 1);
	if (type == 2)
		return (-1);
	if (type == 0)
		return (envx_add((t_kstr){entry, var_length + name_length}, env));
	env_entry.length = ft_strlen(env_entry.ptr);
	if (type == 3)
		return (envx_append(env_entry, (t_kstr){entry + name_length, var_length}, index, env));
	return (envx_replace(env_entry, (t_kstr){entry, name_length + var_length}, index, env));
}
