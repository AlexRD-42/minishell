/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arrows.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:20:48 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/18 20:22:11 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "read_input.h"

int	move_cursor_left(t_line_editor *data)
{
	int	old_y;
	int	i;

	if (data->cursor_pos == 0)
		return (0);
	data->cursor_pos--;
	old_y = data->cursor_y;
	update_cursor_position(data);
	if (data->cursor_y != old_y)
	{
		write(STDOUT_FILENO, "\033[A", 3);
		if (data->cursor_x > 0)
		{
			i = 0;
			while (i++ < data->screen_cols)
				write(STDOUT_FILENO, "\033[1G", 4);
		}
		else
			write(STDOUT_FILENO, "\r", 1);
	}
	else
		write(STDOUT_FILENO, "\033[D", 3);
	return (0);
}

int	move_cursor_right(t_line_editor *data)
{
	int	old_y;

	if (data->cursor_pos >= data->line.length)
		return (0);
	data->cursor_pos++;
	old_y = data->cursor_y;
	update_cursor_position(data);
	if (data->cursor_y != old_y)
	{
		write(STDOUT_FILENO, "\033[B", 3);
		write(STDOUT_FILENO, "\r", 1);
	}
	else
		write(STDOUT_FILENO, "\033[C", 3);
	return (0);
}

int	stt_history_prev(t_line_editor *data)
{
	t_hst	*hist;
	char	temp[FT_LINE_MAX];
	size_t	len;

	hist = data->history;
	if (hist->count == 0)
		return (0);
	if (data->hist_current == hist->count)
		data->hist_current = hist->count - 1;
	else if (data->hist_current > 0)
		data->hist_current--;
	else
		return (0);
	len = hst_read(data->hist_current, temp, hist);
	if (len == SIZE_MAX)
		return (0);
	ft_memcpy(data->line.ptr, temp, len);
	data->line.length = len;
	data->line.ptr[len] = '\0';
	data->cursor_pos = len;
	redraw_line(data);
	return (0);
}

int	stt_history_next(t_line_editor *data)
{
	t_hst	*hist;
	char	temp[FT_LINE_MAX];
	size_t	len;

	hist = data->history;
	if (hist->count == 0 || data->hist_current >= hist->count)
		return (0);
	data->hist_current++;
	if (data->hist_current >= hist->count)
	{
		data->line.length = 0;
		data->line.ptr[0] = '\0';
		data->cursor_pos = 0;
		redraw_line(data);
		return (0);
	}
	len = hst_read(data->hist_current, temp, hist);
	if (len == SIZE_MAX)
		return (0);
	ft_memcpy(data->line.ptr, temp, len);
	data->line.length = len;
	data->line.ptr[len] = '\0';
	data->cursor_pos = len;
	redraw_line(data);
	return (0);
}

int	handle_arrows(t_line_editor *data)
{
	char	seq[2];

	if (read_key(&seq[0]) != 1)
		return (0);
	if (read_key(&seq[1]) != 1)
		return (0);
	if (seq[0] != '[')
		return (0);
	if (seq[1] == 'A')
		return (stt_history_prev(data));
	else if (seq[1] == 'B')
		return (stt_history_next(data));
	else if (seq[1] == 'C')
		return (move_cursor_right(data));
	else if (seq[1] == 'D')
		return (move_cursor_left(data));
	return (0);
}
