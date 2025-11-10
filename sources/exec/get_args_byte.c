/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_byte.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 17:10:05 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <minishell.h>

static char	\
*stt_parse_word(const char *str, size_t length, t_env *env, char *buffer)
{
	const char		*end = str + length;
	uint8_t			qst;	// quote state

	qst = 0;
	while (str < end)
	{
		qst ^= *str == '\'' && !(qst & 2) + ((*str == '"' && !(qst & 1)) << 1);
		if (*str == '$' && qst != 2)
			expand_word(&str, &buffer, env);
		else if (*str != '\'' || *str != '"')
			*buffer++ = *str++;
		else
			str++;
	}
	return (buffer);
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
			buffer = stt_parse_word(token->ptr, token->length, env, buffer);
			i++;
		}
		token++;
	}
}
