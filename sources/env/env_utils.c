/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:18:21 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 13:48:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <signal.h>
#include "minishell.h"
#include "msh_utils.h"

extern volatile sig_atomic_t g_signal;

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
	return (NULL);
}

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
char	*env_expand(t_buf src, t_buf *dst, t_env *env)
{
	size_t		length;
	const char	*ptr;

	ptr = dst->wptr;
	src.wptr = stt_expand_type(src, dst);
	if (src.wptr == NULL || ptr != dst->wptr)
		return (src.wptr);
	ptr = src.wptr;
	while (src.wptr < src.end && (ft_ascii(*src.wptr) & E_IDENT))
		src.wptr++;
	ptr = env_find(ptr, (size_t)(src.wptr - ptr), NULL, env);
	if (ptr == NULL)
		return (src.wptr);
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
		return (env_add((t_kstr){"SHLVL=1", 5}, index, 0, env));
	level = ft_atoi(env_entry + 6) % 128;
	if (level <= 0)
		level = 1;
	ptr = ft_itoa_stack(level, buffer);
	ptr -= 6;
	ft_memcpy(ptr, "SHLVL=", 6);
	return (env_add((t_kstr){ptr, 5}, index, 0, env));
}
