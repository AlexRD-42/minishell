/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:31:49 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 12:25:21 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_readline.h"
#include <unistd.h>

void	cursor_home(t_line_editor *data)
{
	while (data->cursor_pos)
		move_cursor_left(data);
	data->cursor.col = 0;
	data->cursor.row = 0;
}

void	cursor_end(t_line_editor *data)
{
	while (data->cursor_pos < data->line.length)
		move_cursor_right(data);
}

void	redraw_line(t_line_editor *data)
{
	size_t	saved_pos;
	size_t	i;

	saved_pos = data->cursor_pos;
	if (data->cursor_pos > 0
		&& (data->cursor_pos + data->prompt.length) % data->screen.col == 0)
		write(STDOUT_FILENO, " \r", 2);
	cursor_home(data);
	write(STDOUT_FILENO, "\033[0J", 4);
	write(STDOUT_FILENO, data->line.ptr, data->line.length);
	if (data->line.length > 0
		&& (data->line.length + data->prompt.length) % data->screen.col == 0)
		write(STDOUT_FILENO, " \r", 2);
	data->cursor_pos = data->line.length;
	data->cursor.col = (data->prompt.length + data->line.length)
		% data->screen.col;
	i = data->line.length;
	while (i > saved_pos)
	{
		move_cursor_left(data);
		i--;
	}
}
