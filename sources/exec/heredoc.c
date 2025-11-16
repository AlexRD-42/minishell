/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 19:45:20 by adeimlin         ###   ########.fr       */
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

static uint8_t	\
stt_find_eof(const char **line_ptr, const char *end, const char *eof, size_t eof_len)
{
	const char	*ptr = *line_ptr;

	end -= eof_len;
	while (ptr < end)
	{
		*line_ptr = ptr;
		if (stt_eof_cmp(ptr, eof) == 1)
			return (1);
		else
			ptr++;
		while (ptr < end && *ptr != '\n')
			ptr++;
	}
	return (0);
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
size_t	stt_read_input(const char *eof, char *buffer)
{
	ssize_t			bytes_read;
	char			*wptr;
	const size_t	eof_len = ft_strlen(eof);
	const char 		*line_ptr = buffer;

	bytes_read = read(STDIN_FILENO, buffer, FT_PAGE_SIZE);
	wptr = buffer + bytes_read * (bytes_read > 0);
	while (bytes_read > 0 && wptr < buffer + FT_PIPE_SIZE)
	{
		if (stt_find_eof(&line_ptr, wptr, eof, eof_len) == 1)
			return ((size_t)(line_ptr - buffer));
		bytes_read = read(STDIN_FILENO, wptr, FT_PAGE_SIZE);
		wptr += bytes_read;
	}
	return (SIZE_MAX);
}

// To do: better error handling
int	heredoc(t_token *token, t_vecp *env)
{
	char	buffer[FT_PIPE_SIZE + FT_PAGE_SIZE];
	char	aux_buffer[FT_PIPE_SIZE];
	char	eof[FT_NAME_MAX];
	size_t	length;
	t_buf	buf;

	buf = (t_buf){eof, eof + sizeof(eof), eof};
	if (expand_token(token, env, &(t_vecp){{buf}, 0, 1, NULL}))
		return (-1);
	length = stt_read_input(eof, buffer);
	if (parse_interval(buffer, buffer + length, env, ))
		return (-1);
	return (stt_write_to_pipe(aux_buffer, arg.offset));
}
