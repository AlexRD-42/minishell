/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:16:19 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/18 10:59:29 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/errno-base.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "minishell.h"

static inline
uint8_t	eof_cmp(const char *str, const char *eof)
{
	while (*str == *eof && *eof != 0)
	{
		str++;
		eof++;
	}
	return (*eof == 0 && (*str == 0 || *str == '\n'));
}

// Can be optimized to search for first /n prior to length
// Remove the ternary dumbass
size_t	is_eof(const char *str, const char *eof, size_t eof_len, size_t length)
{
	static size_t	start = 0;
	size_t			i;

	i = start;
	if (length <= eof_len)
		return (SIZE_MAX);
	length -= eof_len;
	while (eof_cmp(str, eof) == 0 && i < length)
	{
		while (str[i] != '\n' && i < length)
			i++;
		if (str[i] == '\n')
			start = ++i;
	}
	return (i < length ? i : SIZE_MAX);
}

int	write_to_buffer(char *buffer, size_t length)
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

// First write everything from buffer to disk
// Then do normal read and write appends to file
int	write_to_disk(char *buffer, size_t length)
{
	
}

// <<'EOF', <<"EOF", or <<\EOF (delimiter quoted in any way): No expansions
// EOF is always taken literally
// There is no need for length if buffer is zeroed

// Create a GNL with (fd, *buffer, buffer_size) 
int	heredoc(const char *eof)
{
	ssize_t			bytes_read;
	size_t			offset;
	const size_t	eof_length = ft_strlen(eof);
	char			buffer[65536];
	size_t			length;

	offset = 0;
	bytes_read = read(STDIN_FILENO, buffer, PAGE_SIZE);
	while (bytes_read > 0 && offset < 65536 - PAGE_SIZE)
	{
		offset += (size_t) bytes_read;
		length = is_eof(buffer, eof, eof_length, offset);
		if (length != SIZE_MAX)
			return (write_to_buffer(buffer, length));
		bytes_read = read(STDIN_FILENO, buffer + offset, PAGE_SIZE);
	}
	length = is_eof(buffer, eof, eof_length, offset);
	if (length != SIZE_MAX)
		return (write_to_buffer(buffer, length));
	return (-1);
}
