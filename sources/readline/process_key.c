/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:35:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 12:27:17 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_readline.h"
#include <unistd.h>

static int	stt_finalize_line(t_line_editor *data)
{
	cursor_end(data);
	data->line.ptr[data->line.length] = '\0';
	write(STDOUT_FILENO, "\n", 1);
	return (1);
}

static int	stt_handle_printable_char(t_line_editor *data, char c)
{
	size_t	i;

	if (data->line.length >= FT_LINE_MAX - 1)
		return (0);
	i = data->line.length;
	while (i > data->cursor_pos)
	{
		data->line.ptr[i] = data->line.ptr[i - 1];
		i--;
	}
	data->line.ptr[data->cursor_pos] = c;
	data->line.length++;
	write(STDOUT_FILENO, &c, 1);
	data->cursor_pos++;
	data->line.ptr[data->line.length] = '\0';
	redraw_line(data);
	return (0);
}

static int	stt_handle_backspace(t_line_editor *data)
{
	size_t	i;

	if (data->cursor_pos == 0)
		return (0);
	move_cursor_left(data);
	i = data->cursor_pos;
	while (i < data->line.length - 1)
	{
		data->line.ptr[i] = data->line.ptr[i + 1];
		i++;
	}
	data->line.length--;
	data->line.ptr[data->line.length] = '\0';
	redraw_line(data);
	return (0);
}

int	process_key(t_line_editor *data, char c)
{
	if (c >= 32 && c < 127)
		return (stt_handle_printable_char(data, c));
	if (c == '\n')
		return (stt_finalize_line(data));
	if (c == 127 || c == 8)
		return (stt_handle_backspace(data));
	if (c == 27)
		return (handle_arrows(data));
	if (c == 4 && data->line.length == 0)
	{
		data->line.length = SIZE_MAX;
		return (-1);
	}
	return (0);
}
