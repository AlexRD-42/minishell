/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_stack.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:07:58 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/15 20:51:03 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

// // Read (with wptr) IF ptr >= wptr, and break if wptr + read_size > buffer_size
// // scan with ptr until a new line is found or until ptr >= wptr
// ssize_t	stt_read(int fd, char *ptr, t_buf *buffer, size_t read_size)
// {
// 	ssize_t	bytes_read;
	
// 	if (ptr < buffer->wptr)
// 		return (buffer->wptr - ptr);
// 	else if (buffer->wptr + read_size >= buffer->end)
// 		return (-1);
// 	bytes_read = read(fd, buffer->wptr, read_size);
// 	if (bytes_read < 0)
// 	{
// 		perror("msh_read: ");
// 		return (-1);
// 	}
// 	buffer->wptr += (size_t) bytes_read;
// 	*(buffer->wptr++) = 0; 
// 	return (bytes_read);
// }

// char	*get_next_line(int fd, t_buf *buffer, size_t read_size)
// {
// 	char	*ptr;
// 	char	*start;

// 	start = buffer->nl_ptr;
// 	ptr = buffer->nl_ptr + 1;
// 	while (stt_read(fd, ptr, buffer, read_size) > 0)
// 	{
// 		while (*ptr != 0 && *ptr != '\n')
// 			ptr++;
// 		if (*ptr == '\n')
// 		{
// 			buffer->nl_ptr = ptr + 1;
// 			return (start);
// 		}
// 	}
// 	return (NULL);
// }
