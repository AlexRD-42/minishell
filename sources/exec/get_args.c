/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 10:16:00 by adeimlin         ###   ########.fr       */
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
*stt_find_interval(const char *str, size_t length, t_env *env, char **buffer)
{
	const char	*end = str + length;

	while (str < end)
	{
		
	}
}

int	msh_build_argv(t_token *token, t_env *env, char *buffer, char **argv)
{
	size_t	i;

	i = 0;
	while ((token->type & (AND | OR | PIPE | END)) == 0)
	{
		if (token->type == WORD)
			argv[i++] = stt_parse_word(token->ptr, token->length, env, &buffer);
		token++;
	}
}
