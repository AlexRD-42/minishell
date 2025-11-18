/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:57:33 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/18 20:57:48 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "read_input.h"

int	stt_finalize_line(t_line_editor *data)
{
	while (data->cursor_pos < data->line.length)
		move_cursor_right(data);
	write(STDOUT_FILENO, "\n", 1);
	data->line.ptr[data->line.length] = '\0';
	return (1);
}

int	stt_handle_printable_char(t_line_editor *data, char c)
{
	size_t	i;

	if (data->line.length >= FT_LINE_MAX - 1)
		return (0);
	if (data->cursor_pos == data->line.length)
	{
		data->line.ptr[data->line.length] = c;
		data->line.length++;
		data->cursor_pos++;
		write(STDOUT_FILENO, &c, 1);
		update_cursor_position(data);
		return (0);
	}
	i = data->line.length;
	while (i > data->cursor_pos)
	{
		data->line.ptr[i] = data->line.ptr[i - 1];
		i--;
	}
	data->line.ptr[data->cursor_pos] = c;
	data->line.length++;
	data->cursor_pos++;
	write(STDOUT_FILENO, data->line.ptr + data->cursor_pos,
          data->line.length - data->cursor_pos);
	i = data->cursor_pos;
    while (i < data->line.length)
    {
        write(STDOUT_FILENO, "\b", 1);
        i++;
    }
	update_cursor_position(data);
	return (0);
}

int	stt_handle_backspace(t_line_editor *data)
{
	size_t	i;

	if (data->cursor_pos == 0)
		return (0);
	if (data->cursor_pos == data->line.length)
	{
		data->line.length--;
		data->cursor_pos--;
		data->line.ptr[data->line.length] = '\0';
		write(STDOUT_FILENO, "\b \b", 3);
		update_cursor_position(data);
		return (0);
	}
	i = data->cursor_pos -1;
	while (i < data->line.length - 1)
	{
		data->line.ptr[i] = data->line.ptr[i + 1];
		i++;
	}
	data->line.length--;
	data->cursor_pos--;
	write(STDOUT_FILENO, "\b", 1);
	write(STDOUT_FILENO, data->line.ptr + data->cursor_pos,
          data->line.length - data->cursor_pos);
    write(STDOUT_FILENO, " ", 1);
	i = data->cursor_pos;
    while (i <= data->line.length)
    {
        write(STDOUT_FILENO, "\b", 1);
        i++;
    }
	update_cursor_position(data);
	return (0);
}

// Return: 0) coninue reading,  -1) control + D on empty line,  1) final line
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
		data->line.length = -1;
		return (-1);
	}
	return (0);
}
