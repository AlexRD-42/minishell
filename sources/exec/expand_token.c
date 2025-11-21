/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 14:00:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

// BUG: Wildcard matching considers asterisks within quotes if one asterisk was outside
// Return: 0) EOF, 1) MATCH, 2) NO MATCH, -1) OOM, -2) readdir error (P)
static
int	stt_directory_read(DIR *dir_stream, const char *pattern, t_vecp *vec)
{
	size_t			length;
	struct dirent	*dir_entry;

	errno = 0;
	dir_entry = readdir(dir_stream);
	if (dir_entry == NULL && errno == 0)
		return (0);
	else if (dir_entry == NULL)
		return (ft_error("msh_readdir: ", NULL, -2));
	if (ft_strwcmp(dir_entry->d_name, pattern) == 1)
	{
		length = ft_strlen(dir_entry->d_name) + 1;
		if (ft_lmcpy(vec->buf.wptr, dir_entry->d_name, length, vec->buf.end))
			return (-1);
		vec->ptr[vec->count++] = vec->buf.wptr;
		vec->buf.wptr += length;
		return (1);
	}
	return (2);
}

// This function reads from a directory, compares against pattern with wildcard matching and 
// saves up to EOF entries inside the buffer supplied by arg.
// Return: >=0) Number of matches until EOF
// Return:	-1) OOM (P), -2) dir function problems (P), -4) exceeded count
static
ssize_t	stt_expand_glob(const char *pattern, t_vecp *vec)
{
	size_t	count;
	DIR		*dir_stream;
	ssize_t	rvalue;

	dir_stream = opendir(".");
	if (dir_stream == NULL)
		return (ft_error("msh_opendir: ", NULL, -2));
	count = 0;
	rvalue = (vec->count + 1 >= vec->max_count);
	rvalue = (rvalue == 0) - (rvalue << 2);
	while (rvalue > 0)
	{
		rvalue = stt_directory_read(dir_stream, pattern, vec);
		if (vec->count + 1 >= vec->max_count)
			rvalue = -4;
		count += (rvalue == 1);
	}
	if (closedir(dir_stream) < 0)
		ft_error("msh_closedir: ", NULL, 0);
	return (rvalue + (ssize_t)((rvalue == 0) * count));
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
			src.wptr = env_expand(src, dst, env);
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
// Finds the interior length of the string between quotes, and copies it literally
// if its a single quote, or performs variable expansion if it isnt
// Return: NULL) OOM, !NULL) OK
static
char	*stt_find_interval(t_buf src, t_env *env, t_buf *dst)
{
	const char	qtype = *src.wptr;
	const char	quoted = (qtype == '"' || qtype == '\'');
	size_t		length;

	if (quoted)
	{
		src.optr = ++src.wptr;
		while (src.wptr < src.end && *src.wptr != qtype)
			src.wptr++;
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

// Return: >=0) OK, -1) OOM, -2) dir function problems, -4) exceeded count;
/* This function expands a token, and globs the result if indicated by the token
The count variable serves as both a way of knowing if ptr is out of bounds, and
as a way of throwing errors for ambiguous redirects when expanding file names;
Saves the final results to arg, and uses a temporary arg_tmp buffer to store
the pattern if necessary*/
ssize_t	expand_token(t_token *token, t_env *env, t_vecp *vec)
{
	t_buf	*dst;
	t_buf	src;
	char	buffer[FT_ARG_SIZE];	// Limit for one argument
	char    *start_ptr;

	src = (t_buf){(char *)token->ptr, (char *)token->ptr + token->length, (char *)token->ptr};
	if (token->type & E_EXPAND)
		dst = &(t_buf){buffer, buffer + sizeof(buffer), buffer};
	else
	{
		dst = &vec->buf;
		start_ptr = dst->wptr;
	}
	while (src.wptr < src.end)
	{
		src.wptr = stt_find_interval(src, env, dst);
		if (src.wptr == NULL)
			return (-1);
		src.optr = src.wptr;
	}
	if (dst->wptr + 1 > dst->end)
		return (-1);
	*(dst->wptr++) = 0;
	if ((token->type & E_EXPAND))
		return (stt_expand_glob(buffer, vec));
	vec->ptr[vec->count] = start_ptr;
	return (1);
}
