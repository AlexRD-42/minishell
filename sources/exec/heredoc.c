/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/14 12:31:14 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/errno-base.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"

static inline uint8_t	\
stt_eof_cmp(const char *str, const char *eof)
{
	while (*str == *eof && *eof != 0)
	{
		str++;
		eof++;
	}
	return (*eof == 0 && (*str == 0 || *str == '\n'));
}

static const char	\
*stt_find_eof(const char *str, const char *eof, const char *end, size_t eof_len)
{
	const char	*nl_pos = NULL;

	end -= eof_len;
	while (str < end)
	{
		nl_pos = str++;
		if (stt_eof_cmp(nl_pos, eof) == 1)
			break ;
		while (str < end && *str != '\n')
			str++;
	}
	return (nl_pos);
}

static int	\
stt_write_to_pipe(const char *buffer, size_t length)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("msh_pipe: ");
		return (-1);
	}
	ft_write(fd[1], buffer, length);
	close(fd[1]);
	return (fd[0]);
}

static
int	stt_heredoc(const char *eof, t_argv *arg, t_env *env, bool mode)
{
	ssize_t		bytes_read;
	size_t		eof_len;
	char		*read_end;
	const char 	*nl_pos = arg->data;

	// eof_len = ft_strlen(token->);
	bytes_read = read(STDIN_FILENO, arg->data, FT_PAGE_SIZE);
	read_end = arg->data + bytes_read * (bytes_read > 0);
	while (bytes_read > 0 && read_end < arg->data + FT_PIPE_SIZE)
	{
		nl_pos = stt_find_eof(nl_pos, eof, read_end, eof_len);
		if (nl_pos != NULL && stt_eof_cmp(nl_pos, eof) == 1)
			return (stt_write_to_pipe(arg->data, (size_t)(nl_pos - arg->data)));
		bytes_read = read(STDIN_FILENO, read_end, FT_PAGE_SIZE);
		read_end += bytes_read;
	}
}

#define FT_EOF_LENGTH 256

int	heredoc(t_token *token, t_env *env)
{
	char		buffer[FT_PIPE_SIZE + FT_PAGE_SIZE];
	char		eof[FT_EOF_LENGTH];
	t_argv		arg;

	arg = (t_argv){0, 0, eof, NULL, eof + sizeof(eof)};
	if (expand_token(token, env, &arg, 1))
		return (-1);
	arg = (t_argv){0, 0, buffer, NULL, buffer + sizeof(buffer)};
	return (stt_heredoc(eof, &arg, env, (token->type & E_EXPAND) != 0));
}
