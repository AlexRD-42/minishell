/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 18:18:07 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

ssize_t	msh_expand_glob(const char *pattern, t_vecp *vec);

static
char	*stt_rename_asterisks(uint8_t *src, uint8_t *end, uint8_t qtype, bool expand)
{
	if (expand)
	{
		while (src < end && *src != qtype)
		{
			if (*src == '*')
				*src |= 128;	// Tag the bit
			src++;
		}
	}
	else
	{
		while (src < end && *src != qtype)
			src++;
	}
	return ((char *)src);
}

// Needs a better name
// Parses the string and performs variable expansion
// saving the results in a separate buffer supplied by argv
// Return: 0) OK, -1) OOM;
int	parse_interval(t_buf src, t_env *env, t_buf *dst)
{
	size_t		length;

	while (src.wptr < src.end)
	{
		if (*src.wptr == '$')
		{
			length = (size_t)(src.wptr - src.optr);
			if (ft_lmcpy(dst->wptr, src.optr, length, dst->end))
				return (-1);
			dst->wptr += length;
			src.wptr = msh_expand_env(src, dst, env);
			if (src.wptr == NULL)
				return (-1);
			src.optr = src.wptr;
		}
		else
			src.wptr++;
	}
	length = (size_t)(src.wptr - src.optr);
	if (ft_lmcpy(dst->wptr, src.optr, length, dst->end))
		return (-1);
	dst->wptr += length;
	return (0);
}

// Needs a better name
// Finds the interior length of the string between quotes, and copies 
// it literally if its a single quote, or performs variable expansion
// Return: NULL) OOM, !NULL) OK
static
char	*stt_find_interval(t_buf src, t_env *env, t_buf *dst, bool expand)
{
	const char	qtype = *src.wptr;
	const bool	quoted = (qtype == '"' || qtype == '\'');
	size_t		length;

	if (quoted)
	{
		src.optr = ++src.wptr;
		src.wptr = stt_rename_asterisks((uint8_t *) src.wptr, (uint8_t *) src.end, (uint8_t) qtype, expand);
	}
	else
		while (src.wptr < src.end && *src.wptr != '"' && *src.wptr != '\'')
			src.wptr++;
	length = (size_t)(src.wptr - src.optr);
	if (qtype == '\'')
	{
		if (ft_lmcpy(dst->wptr, src.optr, length, dst->end))
			return (NULL);
		dst->wptr += length;
	}
	else if (parse_interval((t_buf){src.optr, src.wptr, src.optr}, env, dst))
		return (NULL);
	return (src.wptr + (src.wptr < src.end && quoted));
}

// Return: >=0) OK, -1) OOM (P), -2) dir problems (P), -4) exceeded count;
ssize_t	expand_token(t_token token, t_env *env, t_vecp *vec)
{
	t_buf			*dst;
	t_buf			src;
	char			buffer[FT_ARG_SIZE];	// Limit for one argument
	const uintptr_t	address = (uintptr_t) vec->buf.wptr;

	src = (t_buf){(token.ptr), (token.ptr) + token.length, (token.ptr)};	// Review
	if (token.type & E_EXPAND)
		dst = &(t_buf){buffer, buffer + sizeof(buffer), buffer};
	else
		dst = &vec->buf;
	while (src.wptr < src.end)
	{
		src.wptr = stt_find_interval(src, env, dst, !!(token.type & E_EXPAND));
		if (src.wptr == NULL)
			return (-1);
		src.optr = src.wptr;
	}
	if (dst->wptr + 1 > dst->end)
		return (-1);
	*(dst->wptr++) = 0;				// Check if not better inside parse_interval
	if ((token.type & E_EXPAND))	// Check count == 0
		return (msh_expand_glob(buffer, vec));
	vec->ptr[vec->count++] = (char *) address;
	return (1);
}
/* This function expands a token, and globs the result if indicated by the token
The count variable serves as both a way of knowing if ptr is out of bounds, and
as a way of throwing errors for ambiguous redirects when expanding file names;
Saves the final results to arg, and uses a temporary arg_tmp buffer to store
the pattern if necessary*/