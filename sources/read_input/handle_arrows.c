/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arrows.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:33:52 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/20 19:34:04 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "read_input.h"
#include "msh_utils.h"
#include "minishell.h"

int	move_cursor_left(t_line_editor *data)
{
	if (data->cursor_pos == 0)
		return (0);
	data->cursor_pos--;
	if (data->cursor.col == 0)
	{
		write(STDOUT_FILENO, "\033[A", 3);
		write(STDOUT_FILENO, "\033[999C", 6);
		data->cursor.col = data->screen.col - 1;
		data->cursor.row--;
	}
	else
	{
		write(STDOUT_FILENO, "\033[D", 3);
		data->cursor.col--;
	}
	return (0);
}

int	move_cursor_right(t_line_editor *data)
{
	if (data->cursor_pos >= data->line.length)
		return (0);
	data->cursor_pos++;
	if (data->cursor.col >= data->screen.col - 1)
	{
		write(STDOUT_FILENO, "\033[B", 3);
		write(STDOUT_FILENO, "\r", 1);
		data->cursor.col = 0;
		data->cursor.row++;
	}
	else
	{
		write(STDOUT_FILENO, "\033[C", 3);
		data->cursor.col++;
	}
	return (0);
}

static int	stt_history_prev(t_line_editor *data)
{
	t_hst	*hist;
	char	temp[FT_LINE_MAX];
	size_t	len;

	hist = data->hist;
	if (hist->count == 0 || data->hst_current == 0)
		return (0);
	cursor_home(data);
	write(STDOUT_FILENO, "\033[0J", 4);
	write(STDOUT_FILENO, data->prompt.ptr, data->prompt.length);
	data->hst_current--;
	len = hst_read(data->hst_current, temp, hist);
	if (len == SIZE_MAX)
		return (0);
	ft_memcpy(data->line.ptr, temp, len);
	data->line.length = len;
	data->line.ptr[len] = '\0';
	write(STDOUT_FILENO, data->line.ptr, data->line.length);
	data->cursor_pos = data->line.length;
	data->cursor.col = (data->prompt.length + data->line.length)
		% data->screen.col;
	return (0);
}

static int	stt_history_next(t_line_editor *data)
{
	t_hst	*hist;
	char	temp[FT_LINE_MAX];
	size_t	len;

	hist = data->hist;
	if (hist->count == 0 || data->hst_current >= hist->count)
		return (0);
	cursor_home(data);
	write(STDOUT_FILENO, "\033[0J", 4);
	write(STDOUT_FILENO, data->prompt.ptr, data->prompt.length);
	data->hst_current++;
	if (data->hst_current >= hist->count)
		return (reset_line(data), 0);
	len = hst_read(data->hst_current, temp, hist);
	if (len == SIZE_MAX)
		return (0);
	ft_memcpy(data->line.ptr, temp, len);
	data->line.length = len;
	data->line.ptr[len] = '\0';
	write(STDOUT_FILENO, data->line.ptr, data->line.length);
	data->cursor_pos = data->line.length;
	data->cursor.col = (data->prompt.length + data->line.length)
		% data->screen.col;
	return (0);
}

int	handle_arrows(t_line_editor *data)
{
	char	seq[2];

	if (read_key(&seq[0]) != 1)
		return (0);
	if (seq[0] != '[')
		return (0);
	if (read_key(&seq[1]) != 1)
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
