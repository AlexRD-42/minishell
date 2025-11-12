/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/12 15:34:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <dirent.h>
#include "minishell.h"
#include "msh_types.h"

// Variable name is defined by all letters until not alphanumeric
// This name is used to find the var name in ENV
// Updates str and buffer to the end of their respective copy
static
void	stt_expand_variable(const char **str, t_argv *arg, t_env *env)
{
	size_t		index;
	size_t		length;
	const char	*ptr = *str;

	length = 0;
	while (ft_ascii(ptr[length]) & E_IDENT)
		length++;
	index = env_find(env, *str, length);
	*str += length;
	if (index == SIZE_MAX)
		return ;
	ptr = env->ptr[index];
	while (*ptr != 0 && *ptr != '=')
		ptr++;
	length = 0;
	while (ptr[length] != 0)
		length++;
	ft_memcpy(arg->data + arg->offset, ptr, length);
	arg->offset += length;
}

static
size_t	stt_find_interval(const char *str, const char *end)
{
	const char	*ostr = str;
	char		quote_type;

	if (*str == '"' || *str == '\'')
	{
		quote_type = *str++;
		while (*str != quote_type)	// removed str < end because guaranteed to have matching quote
			str++;
	}
	else
	{
		while (str < end && *str != '"' && *str != '\'')
			str++;
	}
	return ((size_t)(str - ostr));
}

static uint8_t
stt_parse_interval(const char *str, size_t length, t_env *env, t_argv *arg)
{
	const char		*end = str + length;
	const char		*ostr = str;

	str += *str == '"';
	while (str < end)
	{
		if (*str == '$')
		{
			length = (size_t)(str - ostr);
			ft_memcpy(arg->data + arg->offset, ostr, length);
			arg->offset += length;
			stt_expand_variable(&str, arg, env);
			ostr = str;
		}
		else
			str++;
	}
	length = (size_t)(str - ostr);
	ft_memcpy(arg->data + arg->offset, ostr, length);
	arg->offset += length;
	return (0);
}

// Need to fix length issues
// Needs to check for overflow
// Expand word already calls expand_glob if necessary
// Length check isn't a problem for 1 count (max is 256 anyways)
uint8_t	stt_expand_word(t_token *token, t_env *env, t_argv *arg)
{
	const char	*end = token->ptr + token->length;
	const char	*str = token->ptr;
	size_t		interval;

	while (str < end)
	{
		interval = stt_find_interval(str, end);
		if (*str == '\'')
		{
			ft_memcpy(arg->data + arg->offset, ++str, interval - 1);
			arg->offset += (interval - 1);
		}
		else
			stt_parse_interval(str, interval, env, arg);
		str += interval;
	}
	return (0);
}

// This function expands a token, and globs the result if indicated by the token
// The count variable serves as both a way of knowing if ptr is out of bounds, and
// as a way of throwing errors for ambiguous redirects when expanding file names.
// Saves the final results to arg, and uses a temporary arg_word buffer to store
// the pattern if necessary.
uint8_t	expand_token(t_token *token, t_env *env, t_argv *arg, size_t count)
{
	char			buffer[FT_WCARD_SIZE];
	t_argv			arg_word;
	
	if (token->type & E_EXPAND)
	{
		arg_word = (t_argv){0, 0, buffer, NULL};
		stt_expand_word(token, env, &arg_word);
		expand_glob(arg_word.data, arg, count);
	}
	else
	{
		stt_expand_word(token, env, arg);
	}
}
