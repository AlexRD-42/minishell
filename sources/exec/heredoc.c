/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 12:12:18 by adeimlin         ###   ########.fr       */
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
#include "msh_utils.h"

// static uint8_t	\
// stt_find_eof(const char **line_ptr, const char *end, const char *eof, size_t eof_len)
// {
// 	const char	*str = *line_ptr;
// 	size_t		i;

// 	end -= eof_len;
// 	while (str < end)
// 	{
// 		i = 0;
// 		*line_ptr = str;
// 		while (str[i] == eof[i] && i < eof_len)
// 			i++;
// 		if ((i == eof_len) && (str[i] == 0 || str[i] == '\n'))
// 			return (1);
// 		str++;
// 		while (str < end && *str != '\n')
// 			str++;
// 	}
// 	return (0);
// }

// // Truncate or error? And do we null terminate?
// // Maybe error on read, but silent truncation on overflow
// static
// size_t	stt_read_eof(const char *eof, char *buffer)
// {
// 	ssize_t			bytes_read;
// 	char			*wptr;
// 	const size_t	eof_len = ft_strlen(eof);
// 	const char 		*line_ptr = buffer;

// 	bytes_read = read(STDIN_FILENO, buffer, FT_PAGE_SIZE);
// 	wptr = buffer + bytes_read * (bytes_read > 0);
// 	while (bytes_read > 0 && wptr < buffer + FT_PIPE_SIZE)
// 	{
// 		if (stt_find_eof(&line_ptr, wptr, eof, eof_len) == 1)
// 			return ((size_t)(line_ptr - buffer));
// 		bytes_read = read(STDIN_FILENO, wptr, FT_PAGE_SIZE);
// 		wptr += bytes_read;
// 	}
// 	return (SIZE_MAX);
// }

// static int	stt_heredoc(const char *eof, t_env *env, t_fd fd)
// {
// 	char	buffer[FT_PIPE_SIZE + FT_PAGE_SIZE];
// 	char	aux_buffer[FT_PIPE_SIZE];
// 	size_t	length;

// 	length = stt_read_input(eof, buffer);
// 	if (length == SIZE_MAX)
// 		return (-1);
// 	if (parse_interval(buffer, buffer + length, env))
// 		return (-1);
// 	ft_write(fd.out, buffer, length);
// 	close(fd.out);
// 	return (fd.in);
// }

// // To do: better error handling
// // Uses up to 256k of stack memory (can be reduced to 128k if we call expand_token)
// // In a function that doesnt have the big buffers
// int	heredoc(t_token *token, t_env *env)
// {
// 	char	eof[FT_NAME_MAX];
// 	t_buf	buf;
// 	t_fd	fd;

// 	if (pipe(fd.ptr) == -1)
// 	{
// 		perror("msh_pipe: ");
// 		return (-1);
// 	}
// 	buf = (t_buf){eof, eof + sizeof(eof), eof};
// 	if (expand_token(token, env, &(t_vecp){{buf}, 0, 1, NULL}))
// 		return (-1);
// 	stt_heredoc(eof, env);
// 	return (stt_write_to_pipe(aux_buffer, arg.offset));
// }
