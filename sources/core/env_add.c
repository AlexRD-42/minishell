/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:11:43 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 18:27:02 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_utils.h"

char	*allocate_blocks(t_env *env, size_t bytes);
size_t	mark_for_deletion(char *ptr, t_env *env);

static
ssize_t	stt_get_blocks_diff(t_env *env, const char *ptr, size_t total_size)
{
	size_t			i;
	size_t			start;
	char			*metadata;
	const ssize_t	blocks_needed = 1 + total_size / FT_BLOCK_SIZE;
	const size_t	count = env->max_count;

	metadata = env->metadata;
	start = (size_t)(ptr - env->optr) / FT_BLOCK_SIZE;
	i = start + (metadata[start] == E_META_HEAD);
	while (i < count && metadata[i] >= E_META_USED)
		i++;
	return ((ssize_t)(i - start) - blocks_needed);
}

int	env_replace(const char *new_entry, size_t index, t_env *env)
{
	char	*wptr;
	char	*old_entry;
	size_t	total_size;

	old_entry = env->ptr[index];
	total_size = ft_strlen(new_entry) + 1;
	if (stt_get_blocks_diff(env, old_entry, total_size) < 0)
	{
		wptr = allocate_blocks(env, total_size);
		if (wptr == NULL)
			return (ft_error("msh_export: out of memory", "", -1));
		mark_for_deletion(old_entry, env);	// Review: Call before allocate blocks
		ft_memcpy(wptr, new_entry, total_size);
		env->ptr[index] = wptr;
	}
	else
		ft_memcpy(old_entry, new_entry, total_size);
	return (0);
}

int	env_append(const char *new_entry, size_t index, t_env *env)
{
	char			*wptr;
	char			*old_entry;
	size_t			old_length;
	size_t			new_size;
	size_t			total_size;

	old_entry = env->ptr[index];
	old_length = ft_strlen(old_entry);
	new_size = ft_strlen(new_entry) + 1;
	total_size = old_length + new_size;
	if (stt_get_blocks_diff(env, old_entry, total_size) < 0)
	{
		wptr = allocate_blocks(env, total_size);
		if (wptr == NULL)
			return (ft_error("msh_export: out of memory", "", -1));	// OOM
		mark_for_deletion(old_entry, env);		// Prevents aliasing if its after
		old_entry = ft_memcpy(wptr, old_entry, old_length);
		env->ptr[index] = old_entry;	// Reassigns pointer to new block
	}
	old_entry += old_length;
	ft_memcpy(old_entry, new_entry, new_size);
	return (0);
}

// Adds a new entry regardless
int	env_add(const char *new_entry, t_env *env)
{
	char			*wptr;
	const size_t	total_size = ft_strlen(new_entry) + 1;

	wptr = allocate_blocks(env, total_size);
	if (wptr == NULL)
		return (ft_error("msh_export: out of memory", "", -1));
	ft_memcpy(wptr, new_entry, total_size);
	env->ptr[env->count] = wptr;
	env->count++;
	env->ptr[env->count] = NULL;
	return (0);
}

// Checks to see if the result exists in env
// Return: 0) Ok, -1) OOM (P), -2) Append but no match (P)
int	env_export(const char *entry, t_env *env)
{
	size_t	length;
	size_t	index;
	bool	append;

	length = 0;
	index = 0;
	while (entry[length] != 0 && entry[length] != '=')
		length++;
	append = (length > 1) && (entry[length] == '=')	&& entry[length - 1] == '+';
	env_find(entry, length - append, &index, env);
	length += (entry[length] == '=');
	if (index == SIZE_MAX && append == 0)
		return (env_add(entry, env));
	else if (index == SIZE_MAX)
		return (ft_error("msh_export: could not find var to append: ", entry, -2));
	if (append == 0)
		return (env_replace(entry, index, env));
	return (env_append(entry + length, index, env));
}
