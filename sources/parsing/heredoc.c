/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/24 20:43:40 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include "msh_utils.h"

static uint8_t	\
stt_find_eof(const char **line_ptr, const char *end, const char *eof, size_t eof_len)
{
	const char	*str = *line_ptr;
	size_t		i;

	end -= eof_len;
	while (str < end)
	{
		i = 0;
		*line_ptr = str;
		while (i < eof_len && str[i] == eof[i])
			i++;
		if ((i == eof_len) && (str[i] == 0 || str[i] == '\n'))
			return (1);
		while (str < end && *str != '\n')
			str++;
		str += (str < end && *str == '\n');
	}
	return (0);
}

static
size_t	stt_clean_eof(char *dst, const char *src, size_t length)
{
	const char	*end = src + length;
	char		*odst;

	odst = dst;
	while (src < end)
	{
		if (*src != '"' && *src != '\'')
			*dst++ = *src++;
		else
			src++;
	}
	*dst++ = 0;
	return ((size_t)(dst - odst));
}

// Truncate or error? And do we null terminate?
// Maybe error on read, but silent truncation on overflow
static
size_t	stt_read_eof(const char *eof, char *buffer)
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
		bytes_read = read(STDIN_FILENO, wptr, FT_PAGE_SIZE);	// Maybe print errors here
		wptr += bytes_read;
	}
	return (SIZE_MAX);
}

int	stt_heredoc(const char *eof, int32_t fd, bool expand, t_env *env)
{
	char	buffer[FT_PIPE_SIZE + FT_PAGE_SIZE];
	char	aux_buffer[FT_PIPE_SIZE];
	t_buf	dst;
	t_buf	src;
	size_t	length;

	length = stt_read_eof(eof, buffer);
	if (length == SIZE_MAX)
		return (-1);
	buffer[length] = 0;	// Check
	dst = (t_buf){aux_buffer, aux_buffer + sizeof(aux_buffer), aux_buffer};
	src = (t_buf){buffer, buffer + length, buffer};
	if (expand == true)
	{
		if (parse_interval(src, env, &dst) < 0)
			return (-1);
		if (ft_write(fd, dst.optr, (size_t)(dst.wptr - dst.optr)) < 0)
			return (-1);
	}
	else if (ft_write(fd, buffer, length) < 0)
		return (-1);
	return (0);
}

// Returns -1 on failure
int	heredoc(const char *src, size_t length, bool expand, t_env *env)
{
	int32_t	fd[2];
	char	eof[FT_NAME_MAX];
	int		rvalue;

	if (length >= FT_NAME_MAX)
		return (ft_error("msh_heredoc: eof name is too long", "", -1));
	if (pipe(fd) == -1)
		return (ft_error("msh_pipe: ", NULL, -1));
	stt_clean_eof(eof, src, length);
	rvalue = stt_heredoc(eof, fd[1], expand, env);
	close(fd[1]);
	if (rvalue == -1)
	{
		close(fd[0]);
		return (-1);
	}
	else
		return (fd[0]);
}
