/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 12:58:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/11 20:58:11 by adeimlin         ###   ########.fr       */
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
void	stt_expand_variable(const char **str, char **buffer, t_env *env)
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
	ft_memcpy(*buffer, ptr, length);
	*buffer += length;
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

static char \
*stt_parse_interval(const char *str, size_t length, t_env *env, char *buffer)
{
	const char		*end = str + length;
	const char		*ostr = str;

	str += *str == '"';
	while (str < end)
	{
		if (*str == '$')
		{
			length = (size_t)(str - ostr);
			ft_memcpy(buffer, ostr, length);
			buffer += length;
			stt_expand_variable(&str, &buffer, env);
			ostr = str;
		}
		else
			str++;
	}
	length = (size_t)(str - ostr);
	ft_memcpy(buffer, ostr, length);
	return (buffer + length);
}

// Need to fix length issues
char	*expand_word(const char *str, size_t length, t_env *env, char *buffer)
{
	const char	*end = str + length;
	size_t		interval;

	while (str < end)
	{
		interval = stt_find_interval(str, end);
		if (*str == '\'')
		{
			ft_memcpy(buffer, ++str, interval - 1);
			buffer += (interval - 1);
		}
		else
			buffer = stt_parse_interval(str, interval, env, buffer);
		str += interval;
	}
	return (buffer);
}
