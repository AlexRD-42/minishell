/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:47:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 19:32:50 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"

// Attempts to find the entry string in Env vars
// Searches backwards to delete reserved vars last
size_t	env_find(t_vecp *env, const char *entry, size_t length)
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

char	*env_get_entry(t_vecp *env, const char *entry, size_t vlength[2])
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

uint8_t	env_del(t_vecp *env, const char *entry)
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
	env->wptr -= length;
	ft_memmove(ptr, ptr + length, (size_t)(env->wptr - ptr));	// This needs checking
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
static
char	*stt_expand_type(t_buf src, t_buf *dst)
{
	char		*ptr;
	char		buffer[32];
	size_t		length;

	src.wptr += src.wptr < src.end && *src.wptr == '$';
	if (src.wptr < src.end && *src.wptr == '?')
	{
		ptr = ft_itoa_stack(g_signal, buffer + sizeof(buffer));
		length = ft_strlen(ptr);
		if (ft_lmcpy(dst->wptr, ptr, length, dst->end))
			return (NULL);
		dst->wptr += length;
		return (src.wptr + 1);
	}
	else if (src.wptr >= src.end || (ft_ascii(*src.wptr) & E_IDENT) == 0)
	{
		if (dst->wptr + 1 > dst->end)
			return (NULL);
		*(dst->wptr++) = '$';
	}
	return (src.wptr);
}

// Variable name is defined by all letters until not alphanumeric
// This name is used to find the var name in ENV
// Updates str and buffer to the end of their respective copy
// To do: Create an env helper that returns the value rather than the entry
// Return: NULL) OOM, !NULL) OK
char	*env_expand(t_buf src, t_buf *dst, t_vecp *env)
{
	size_t		index;
	size_t		length;
	const char	*ptr;

	ptr = dst->wptr;
	src.wptr = stt_expand_type(src, dst);
	if (src.wptr == NULL || ptr != dst->wptr)
		return (src.wptr);
	ptr = src.wptr;
	while (src.wptr < src.end && (ft_ascii(*src.wptr) & E_IDENT))
		src.wptr++;
	index = env_find(env, ptr, (size_t)(src.wptr - ptr));
	if (index == SIZE_MAX)
		return (src.wptr);
	ptr = env->ptr[index];
	while (*ptr != 0 && *ptr != '=')
		ptr++;
	ptr += *ptr == '=';
	length = 0;
	while (ptr[length] != 0)
		length++;
	if (ft_lmcpy(dst->wptr, ptr, length, dst->end))
		return (NULL);
	dst->wptr += length;
	return (src.wptr);
}
