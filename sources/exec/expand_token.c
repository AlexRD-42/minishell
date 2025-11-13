/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/13 12:37:56 by adeimlin         ###   ########.fr       */
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
// To do: Create an env helper that returns the value rather than the entry
// Return: 0) OK, 1) OOM
static
uint8_t	stt_expand_variable(const char **str, t_argv *arg, t_env *env)
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
		return (0);	// Check
	ptr = env->ptr[index];
	while (*ptr != 0 && *ptr != '=')
		ptr++;
	ptr += *ptr == '=';
	length = 0;
	while (ptr[length] != 0)
		length++;
	if (ft_lmcpy(arg->data + arg->offset, ptr, length, arg->end))
		return (1);
	arg->offset += length;
	return (0);
}

// Returns the interior length of the interval
static
size_t	stt_find_interval(const char *str, const char *end)
{
	const char	*ostr = str + (*str == '"' || *str == '\'');
	char		quote_type;

	if (ostr != str)
	{
		quote_type = *str++;
		while (*str != quote_type)
			str++;
	}
	else
	{
		while (str < end && *str != '"' && *str != '\'')
			str++;
	}
	return ((size_t)(str - ostr));
}

// To do: Fix bugs in the length calculation
// Return: 0) OK, 1) OOM
static uint8_t
stt_parse_interval(const char *str, size_t length, t_env *env, t_argv *arg)
{
	const char	type = (*str == '\'') + ((*str == '"') << 1);
	const char	*end = str + length;
	const char	*ostr = str + (type != 0);

	str += (type == 2) + (type == 1) * length;
	while (str < end)
	{
		if (*str == '$')
		{
			length = (size_t)(str++ - ostr);
			if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
				return (1);
			arg->offset += length;
			if (stt_expand_variable(&str, arg, env))
				return (1);
			ostr = str;
		}
		else
			str++;
	}
	length = (size_t)(str - ostr);
	if (ft_lmcpy(arg->data + arg->offset, ostr, length, arg->end))
		return (1);
	arg->offset += length;
	return (0);
}

// Return: 0) OK, 1) OOM
static
uint8_t	stt_expand_word(t_token *token, t_env *env, t_argv *arg)
{
	const char	*end = token->ptr + token->length;
	const char	*str = token->ptr;
	size_t		interval;

	while (str < end)
	{
		interval = stt_find_interval(str, end);
		if (stt_parse_interval(str, interval, env, arg))
			return (1);
		str += interval;
	}
	return (0);
}

/* This function expands a token, and globs the result if indicated by the token
The count variable serves as both a way of knowing if ptr is out of bounds, and
as a way of throwing errors for ambiguous redirects when expanding file names.
Saves the final results to arg, and uses a temporary arg_tmp buffer to store
the pattern if necessary */
// Return: 0) OK, 1) OOM, 2) dir function problems, 4) exceeded count
uint8_t	expand_token(t_token *token, t_env *env, t_argv *arg, size_t count)
{
	t_argv	*arg_tmp;
	char	buffer[FT_WCARD_SIZE];
	uint8_t	rvalue;

	if (token->type & E_EXPAND)
	{
		arg_tmp = &(t_argv){0, 0, buffer, NULL, buffer + sizeof(buffer)};
		rvalue = stt_expand_word(token, env, arg_tmp);
		if (rvalue == 0)
			rvalue = expand_glob(buffer, arg, count);
	}
	else
		rvalue = stt_expand_word(token, env, arg);
	return (rvalue);
}
