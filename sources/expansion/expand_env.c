/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:18:21 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 09:32:19 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_utils.h"

// Handles solo $ and ?.
// Maybe a good idea to copy null terminator as well, but not index it
static
char	*stt_expand_type(t_buf src, t_buf *dst, int32_t exit_status)
{
	char		*ptr;
	char		buffer[32];
	size_t		length;

	src.wptr += src.wptr < src.end && *src.wptr == '$';
	if (src.wptr < src.end && *src.wptr == '?')
	{
		ptr = ft_itoa_stack(exit_status, buffer + sizeof(buffer) - 1);
		length = ft_strlen(ptr);
		if (ft_lmcpy(dst->wptr, ptr, length + 1, dst->end))
			return (NULL);
		dst->wptr += length;
		return (src.wptr + 1);
	}
	else if (src.wptr >= src.end || (ft_ascii(*src.wptr) & E_IDENT) == 0)
	{
		if (dst->wptr + 1 > dst->end)
			return (NULL);
		*(dst->wptr++) = '$';
		*dst->wptr = 0;		// Review
	}
	return (src.wptr);
}

// Variable name is defined by all letters until not alphanumeric
// This name is used to find the var name in ENV
// Updates str and buffer to the end of their respective copy
// Return: NULL) OOM, !NULL) OK
char	*msh_expand_env(t_buf src, t_buf *dst, t_env *env)
{
	size_t		length;
	const char	*ptr;

	ptr = dst->wptr;
	src.wptr = stt_expand_type(src, dst, env->exit_status);
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
	if (ft_lmcpy(dst->wptr, ptr, length + 1, dst->end))
		return (NULL);
	dst->wptr += length;
	return (src.wptr);
}
