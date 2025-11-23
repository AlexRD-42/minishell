/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_byte.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/11 16:10:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_types.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <minishell.h>
#include <fcntl.h>

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

// APPEND, REDIR_IN, REDIR_OUT
static int32_t
stt_parse_fd(t_token *token, t_env *env, char *buffer)
{
	char	*str;
	int		fd;
	int32_t	flags;

	flags = O_RDONLY;
	str = stt_parse_word(token->ptr, token->length, env, buffer);
	if ((token->type & E_REDIR_IN) == 0)
	{
		if (token->type & E_APPND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else if (token->type & E_REDIR_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(str, flags, 0644);
	}
	else
		fd = open(str, flags);
	if (fd < 0)
		//
}

int	msh_build_argv(t_token *token, t_env *env, char *buffer, char **argv)
{
	char	buffer2[4096];
	size_t	i;

	i = 0;
	while ((token->type & E_CMD_END) == 0)
	{
		if (token->type == E_WORD)
		{
			argv[i] = buffer;
			buffer = stt_parse_word(token->ptr, token->length, env, buffer);
			i++;
		}
		if (token->type == E_FILE)
		{
			token->fd[0] = stt_parse_fd(token, env, buffer2);
		}
		token++;
	}
}
