/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 12:38:48 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/errno-base.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "minishell.h"

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
		perror("pipe");
		return (-1);
	}
	ft_write(fd[1], buffer, length);
	close(fd[1]);
	return (fd[0]);
}

// Check for UB in execution
// To check: is \nEOF\0 valid?
// Needs variable expansion
int	heredoc(t_token *token)
{
	ssize_t		bytes_read;
	size_t		eof_len;
	char		buffer[FT_HDOC_SIZE + FT_PAGE_SIZE];
	char		*read_end;
	const char 	*nl_pos = buffer;

	if (eof == NULL)
		return (-1);
	eof_len = ft_strlen(eof);
	bytes_read = read(STDIN_FILENO, buffer, FT_PAGE_SIZE);
	read_end = buffer + bytes_read * (bytes_read > 0);
	while (bytes_read > 0 && read_end < buffer + FT_HDOC_SIZE)
	{
		nl_pos = stt_find_eof(nl_pos, eof, read_end, eof_len);
		if (nl_pos != NULL && stt_eof_cmp(nl_pos, eof) == 1)
			return (stt_write_to_pipe(buffer, (size_t)(nl_pos - buffer)));
		bytes_read = read(STDIN_FILENO, read_end, FT_PAGE_SIZE);
		read_end += bytes_read;
	}
	return (-1);
}
