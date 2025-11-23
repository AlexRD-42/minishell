/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 22:46:43 by adeimlin         ###   ########.fr       */
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
		while (str[i] == eof[i] && i < eof_len)
			i++;
		if ((i == eof_len) && (str[i] == 0 || str[i] == '\n'))
			return (1);
		str++;
		while (str < end && *str != '\n')
			str++;
	}
	return (0);
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
		bytes_read = read(STDIN_FILENO, wptr, FT_PAGE_SIZE);
		wptr += bytes_read;
	}
	return (SIZE_MAX);
}

int	stt_heredoc(t_buf eof, t_env *env, int32_t fd[2])
{
	char	buffer[FT_PIPE_SIZE + FT_PAGE_SIZE];
	char	aux_buffer[FT_PIPE_SIZE];
	t_buf	dst;
	t_buf	src;
	size_t	length;

	dst = (t_buf){aux_buffer, aux_buffer + sizeof(aux_buffer), aux_buffer};
	length = stt_read_eof(eof.optr, buffer);
	if (length == SIZE_MAX)
		return (-1);
	src = (t_buf){buffer, buffer + length, buffer};
	if (parse_interval(src, env, &dst))
		return (-1);
	*dst.wptr++ = 0;	// Check
	ft_write(fd[1], dst.optr, (size_t)(dst.wptr - dst.optr));	// Error checking
	close(fd[1]);
	return (fd[0]);
}

// To do: better error handling
int	heredoc(t_token *token, t_env *env)
{
	char	eof[FT_NAME_MAX];
	char	*ptr[1];
	int32_t	fd[2];
	t_buf	eof_buf;

	if (pipe(fd) == -1)
		ft_error("msh_pipe: ", NULL, -1);
	eof_buf = (t_buf){eof, eof + sizeof(eof), eof};
	if (expand_token(*token, env, &(t_vecp){eof_buf, 0, 1, ptr}))	// Review: Check conditional expand
		return (-1);
	return (stt_heredoc(eof_buf, env, fd));
}
