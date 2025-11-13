/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscalls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 11:59:05 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/12 21:51:17 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

// ft_pipe, ft_open, ft_fork, ft_close, 

// To do: Error handling, set errno (EIO)?
// Repeats the write until the buffer is fully consumed, and repeats
// The write when a signal interrupts it.
ssize_t	ft_write(int fd, const void *buffer, size_t length)
{
	ssize_t			bytes_written;
	size_t			bytes_remaining;
	const uint8_t	*ptr = (const uint8_t *) buffer;

	bytes_remaining = length;
	while (bytes_remaining > 0)
	{
		bytes_written = write(fd, ptr, bytes_remaining);
		if (bytes_written > 0)
		{
			bytes_remaining -= (size_t) bytes_written;
			ptr += (uintptr_t) bytes_written;
		}
		else if (errno != EINTR || bytes_written == 0)
			return (-1);
	}
	return ((ssize_t) length);
}
