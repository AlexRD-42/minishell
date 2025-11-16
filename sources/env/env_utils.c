/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:47:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 13:58:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"

// Attempts to find the entry string in Env vars
// Searches backwards to delete reserved vars last
size_t	env_find(t_env *env, const char *entry, size_t length)
{
	size_t	i;
	size_t	j;

	if (length == SIZE_MAX)
	{
		length = 0;
		while (entry[length] != 0 && entry[length] != '=')
			length++;
	}
	if (length == 0)
		return (SIZE_MAX);
	i = env->count + (env->count == 0);
	while (i-- > 0)
	{
		j = 0;
		while (j < length && env->ptr[i][j] == entry[j])
			j++;
		if (j == length && env->ptr[i][j] == '=')
			return (i);
	}
	return (SIZE_MAX);
}

char	*env_get_entry(t_env *env, const char *entry, size_t vlength[2])
{
	size_t	index;
	char	*ptr;
	size_t	i;
	size_t	j;

	j = 0;
	while (ft_ascii(entry[j]) & E_IDENT)
		j++;
	vlength[0] = j;
	vlength[1] = 0;
	index = env_find(env, entry, j);
	if (index == SIZE_MAX)
		return (NULL);
	ptr = env->ptr[index];
	i = 0;
	while (ptr[i] != 0 && ptr[i] != '=')
		i++;
	ptr += i + (ptr[i] == '=');
	i = 0;
	while (ptr[i] != 0)
		i++;
	vlength[1] = i;
	return (ptr);
}

uint8_t	env_del(t_env *env, const char *entry)
{
	char	*ptr;
	size_t	index;
	size_t	length;

	index = env_find(env, entry, SIZE_MAX);
	if (index == SIZE_MAX)
		return (1); // Could not find var
	if (index <= 3)
	{
		ft_memset(env->ptr[index], 0, FT_PATH_SIZE);
		return (0);
	}
	ptr = env->ptr[index];
	length = ft_strlen(ptr) + 1;
	env->offset -= length;
	ft_memmove(ptr, ptr + length, env->offset - (size_t)(ptr - env->data));
	while (index < env->count - 1)
	{
		env->ptr[index] = env->ptr[index + 1] - length;
		index++;
	}
	env->ptr[index] = NULL;
	env->count--;
	return (0);
}

extern int	g_signal;

// Handles solo $ and ?
static const
char	*stt_expand_type(const char *str, const char *end, t_argv *arg)
{
	char		*ptr;
	char		buffer[32];
	size_t		length;

	str += str < end && *str == '$';
	if (str < end && *str == '?')
	{
		ptr = ft_itoa_stack(g_signal, buffer + sizeof(buffer));
		length = ft_strlen(ptr);
		if (ft_lmcpy(arg->data + arg->offset, ptr, length, arg->end))
			return (NULL);
		arg->offset += length;
		return (str + 1);
	}
	else if (str >= end || (ft_ascii(*str) & E_IDENT) == 0)
	{
		if (arg->data + arg->offset + 1 > arg->end)
			return (NULL);
		arg->data[arg->offset++] = '$';
	}
	return (str);
}

// Variable name is defined by all letters until not alphanumeric
// This name is used to find the var name in ENV
// Updates str and buffer to the end of their respective copy
// To do: Create an env helper that returns the value rather than the entry
// Return: NULL) OOM, !NULL) OK
const char	*env_expand(const char *str, const char *end, t_argv *arg, t_env *env)
{
	size_t		index;
	size_t		length;
	const char	*ptr;

	length = arg->offset;
	str = stt_expand_type(str, end, arg);
	if (str == NULL || length != arg->offset)
		return (str);
	ptr = str;
	while (str < end && (ft_ascii(*str) & E_IDENT))
		str++;
	index = env_find(env, ptr, (size_t)(str - ptr));
	if (index == SIZE_MAX)
		return (str);
	ptr = env->ptr[index];
	while (*ptr != 0 && *ptr != '=')
		ptr++;
	ptr += *ptr == '=';
	length = 0;
	while (ptr[length] != 0)
		length++;
	if (ft_lmcpy(arg->data + arg->offset, ptr, length, arg->end))
		return (NULL);
	arg->offset += length;
	return (str);
}
