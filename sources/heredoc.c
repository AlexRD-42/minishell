/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/18 16:13:01 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/errno-base.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "minishell.h"

// Possible UB
static inline
uint8_t	stt_eof_cmp(const char *str, const char *eof)
{
	while (*str == *eof && *eof != 0)
	{
		str++;
		eof++;
	}
	return (*eof == 0 && (*str == 0 || *str == '\n'));
}

// Possible infinite loop
static
size_t	stt_find_eof(const char *str, const char *eof, size_t eof_length, size_t length)
{
	size_t	i;

	i = 0;
	while (i + eof_length < length)
	{
		if (stt_eof_cmp(str + i, eof) == 1)
			return (i);
		while (i < length && str[i] != '\n')
			i++;
	}
	return (SIZE_MAX);
}

static
int	stt_write_to_pipe(char *buffer, size_t length)
{
	int	fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	write(fd[1], buffer, length);
	close(fd[1]);
	return (fd[0]);
}

int	heredoc(const char *eof)
{
	ssize_t			bytes_read;
	size_t			offset;
	size_t			length;
	char			buffer[65536];
	const size_t	eof_length = ft_strlen(eof);

	offset = 0;
	bytes_read = read(STDIN_FILENO, buffer, PAGE_SIZE);
	while (bytes_read > 0 && offset < 65536 - PAGE_SIZE)
	{
		length = stt_find_eof(buffer + offset, eof, eof_length, (size_t) bytes_read);
		if (length != SIZE_MAX)
			return (stt_write_to_pipe(buffer, length + offset));
		offset += (size_t) bytes_read;
		bytes_read = read(STDIN_FILENO, buffer + offset, PAGE_SIZE);
	}
	length = stt_find_eof(buffer, eof, eof_length, offset);
	if (length != SIZE_MAX)
		return (stt_write_to_pipe(buffer, length));
	return (-1);
}
