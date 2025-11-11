/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 17:10:05 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <minishell.h>

static char	\
*stt_parse_word(const char *str, size_t length, t_env *env, char **buffer)
{
	const uint8_t	type = (*str == '\'') + ((*str == '"') << 1);
	const char		*end = str + length;
	const char		*ostr;

	str += type > 0;
	if (type == 1)
	{
		ft_memcpy(*buffer, str, length - 1);
		return (*buffer + length - 1);
	}
	ostr = str;
	while (str < end)
	{
		if (*str == '$')
			expand_word(&str, buffer, env);
		str++;
	}
	length = (size_t)(str - ostr);
	ft_memcpy(*buffer, ostr, length); // Copies tail
	return (*buffer + length);
}

// Finds things between quotes
static char	\
*stt_find_interval(const char *str, size_t length)
{
	const char	*end = str + length;
	char		quote;
	const char	*close;

	if (*str == '"' || *str == '\'')
	{
		quote = *str;
		close = str + 1;
		while (close < end && *close != quote)
			close++;
		return ((char *)(close + 1));
	}
	while (str < end && *str != '"' && *str != '\'')
		str++;
	return ((char *)str);
}

int	msh_build_argv(t_token *token, t_env *env, char *buffer, char **argv)
{
	size_t	i;

	i = 0;
	while ((token->type & E_STUSEP) == 0)
	{
		if (token->type == E_WORD)
			argv[i++] = stt_parse_word(token->ptr, token->length, env, &buffer);
		token++;
	}
}
/* PARTE FEITA FELIPE */
//
//
//
//

static char \
*stt_parse_interval(const char *str, size_t length, t_env *env, char **buffer)
{
    const uint8_t   type = (*str == '\'') + ((*str == '"') << 1);
    const char      *end = str + length;
    const char      *ostr;

    str += type > 0;
    length -= (type > 0) * 2;
    if (type == 1)
    {
        ft_memcpy(*buffer, str, length);
        return (*buffer + length);
    }
    ostr = str;
	end -= type > 0;
    while (str < end)
    {
        if (*str == '$')
            expand_word(&str, buffer, env);
		str++;
    }
	length = (size_t)(str - ostr);
	ft_memcpy(*buffer, ostr, length);
    return (*buffer + length);
}

static char	\
*stt_find_interval(const char *str, size_t length)
{
	const char	*end = str + length;
	char		quote;
	const char	*close;

	if (*str == '"' || *str == '\'')
	{
		quote = *str;
		close = str + 1;
		while (close < end && *close != quote)
			close++;
		return ((char *)(close + 1));
	}
	while (str < end && *str != '"' && *str != '\'')
		str++;
	return ((char *)str);
}

static char	\
*stt_parse_word(const char *str, size_t length, t_env *env, char **buffer)
{
	const char		*ptr = str;
	const char		*end = str + length;
	const char		*next;
	size_t			interval;

	while (ptr < env)
	{
		next = stt_find_interval(ptr, end - ptr);
		interval = next - ptr;
		buffer = stt_parse_interval(ptr, interval, env, buffer);
		ptr = next;
	}
	return (*buffer);
}

int	msh_build_argv(t_token *token, t_env *env, char *buffer, char **argv)
{
	size_t	i;

	i = 0;
	while ((token->type & E_STUSEP) == 0)
	{
		if (token->type == E_WORD)
		{
			argv[i] = buffer;
			buffer = stt_parse_word(token->ptr, token->length, env, &buffer);
			*buffer = '\0';
			buffer++;
			i++;
		}
		token++;
	}
	argv[i] = NULL;
	return (i);
}
