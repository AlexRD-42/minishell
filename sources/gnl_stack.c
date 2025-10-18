/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_stack.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:07:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/18 10:48:37 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef PAGE_SIZE
# define PAGE_SIZE 4096
#endif

static
ssize_t	stt_read(int fd, char *buffer, size_t buffer_size, size_t *end)
{
	static size_t	read_end = 0;
	size_t			read_size;

	read_size = buffer_size - *end;
	if (read_size <= 0)
		return (-1);
	if (*end >= read_end)
		return (read(fd, buffer + *end, read_size));
	return (1);
}

// You send a buffer[64k], you receive a char *ptr to the start of the next new line
// Just alter the last /n to be a 0. Upon entry, the null terminator is changed back to \n
// If you exceed the buffer, what happens
char	*get_next_line(int fd, char *buffer, size_t buffer_size, size_t *end)
{
	ssize_t			bytes_read;

	if (buffer[*end] == 0 && *end != 0)
	while (*end < buffer_size)
	{
		bytes_read = read(fd, buffer + *end, PAGE_SIZE);
		*end += (size_t) bytes_read;
		
	}
}
