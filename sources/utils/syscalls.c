/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscalls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:59:05 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 14:01:08 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "msh_utils.h"
#include "msh_defines.h"

// To do: Error handling, set errno (EIO)?
// To do: Same for open, close and other utils
// Repeats the write until the buffer is fully consumed, and repeats
// The write when a signal interrupts it.
ssize_t	ft_write(int fd, const void *buffer, size_t length)
{
	ssize_t			bytes_written;
	size_t			bytes_remaining;
	const uint8_t	*ptr = (const uint8_t *) buffer;
	size_t			i;

	i = 0;
	bytes_remaining = length;
	while (bytes_remaining > 0)
	{
		bytes_written = write(fd, ptr, bytes_remaining);
		if (bytes_written > 0)
		{
			bytes_remaining -= (size_t) bytes_written;
			ptr += (size_t) bytes_written;
		}
		else if (errno != EINTR || bytes_written == 0 || i >= FT_SYSCALL_RETRIES)
			return (ft_error("msh_write: ", NULL, -1));
		else
			i++;
	}
	return ((ssize_t) length);
}

// % 512 is just to make extra sure of no buffer overflows
// If suffix is NULL, suffix becomes strerror(errno)
// Otherwise just send ""
ssize_t	ft_error(const char *prefix, const char *suffix, ssize_t rvalue)
{
	char			buffer[1024];
	const size_t	prefix_length = ft_strlen(prefix) % 512;
	size_t			error_length;
	size_t			total_length;

	ft_memcpy(buffer, prefix, prefix_length);
	if (suffix == NULL)
		suffix = strerror(errno);
	error_length = ft_strlen(suffix) % 512;
	ft_memcpy(buffer + prefix_length, suffix, error_length);
	total_length = prefix_length + error_length;
	buffer[total_length++] = '\n';
	ft_write(STDERR_FILENO, buffer, total_length);
	return (rvalue);
}

// Returns -1 on OOM
ssize_t	ft_read(int fd, void *buffer, void *end, size_t read_size)
{
	ssize_t	bytes_read;
	size_t	bytes_remaining;
	uint8_t	*ptr;
	size_t	i;

	ptr = (uint8_t *) buffer;
	if (ptr + read_size > (uint8_t *) end)
		return (-1);
	i = 0;
	bytes_remaining = read_size;
	while (bytes_remaining > 0)
	{
		bytes_read = read(fd, ptr, bytes_remaining);
		if (bytes_read > 0)
		{
			bytes_remaining -= (size_t) bytes_read;
			ptr += (size_t) bytes_read;
		}
		else if (errno != EINTR || bytes_read == 0 || i >= FT_SYSCALL_RETRIES)
			return (ft_error("msh_read: ", NULL, -1));
		else
			i++;
	}
	return ((ssize_t) ptr - (ssize_t) buffer);
}

// int	ft_open(const char *path, int flags, mode_t mode)
// {
// 	int		fd;
// 	size_t	retries;

// 	retries = 0;
// 	while (1)
// 	{
// 		fd = open(path, flags, mode);
// 		if (fd >= 0)
// 			return (fd);
// 		if (errno != EINTR || retries >= FT_SYSCALL_RETRIES)
// 			return (ft_error("msh_open: ", -1));
// 		retries++;
// 	}
// }
