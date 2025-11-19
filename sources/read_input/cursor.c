/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 15:39:31 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 21:20:33 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "read_input.h"
#include "msh_types.h"
#include "msh_utils.h"


void	rep_cmd(size_t count, const char cmd)
{
	char	num_ptr[32];
	char	buffer[32];
	char	*ptr;
	char	*buffer_ptr;

	buffer_ptr = buffer;
	*buffer_ptr++ = '\033';
	*buffer_ptr++ = '[';
	ptr = ft_itoa_stack((int64_t) count, num_ptr);
	while (*ptr != 0)
		*buffer_ptr++ = *ptr++;
	*buffer_ptr++ = cmd;
	write(STDOUT_FILENO, buffer, (size_t)(buffer_ptr - buffer));
}

void	redraw_line(t_line_editor *data)
{
	size_t	total_len;
	int		end_row;
	int		target_row;

	write(STDOUT_FILENO, "\033[0J", 4);
	write(STDOUT_FILENO, data->prompt.ptr, data->prompt.length);
	write(STDOUT_FILENO, data->line.ptr, data->line.length);
	total_len = data->prompt.length + data->line.length;
	end_row = total_len / data->screen.col;
	target_row = data->cursor.row;
	rep_cmd((size_t)(end_row - target_row), 'A');
	write(STDOUT_FILENO, "\r", 1);
	if (data->cursor.col > 0)
		rep_cmd(data->cursor.col, 'C');
}
