/* ************************************************************************** */
/* handle_arrows.c                                    :+:      :+:    :+:   */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"
#include "msh_utils.h"
#include "read_input.h"

int	move_cursor_left(t_line_editor *data)
{
	if (data->cursor_pos == 0)
		return (0);
	if (data->cursor.col == 0)
	{
		write(STDOUT_FILENO, "\033[A", 3);
		write(STDOUT_FILENO, "\033[999C", 6);	// Hardcode
		data->cursor.col = data->screen.col;
		data->cursor.row -= (data->cursor.row != 0);
	}
	else
	{
		data->cursor.col -= 1;
		write(STDOUT_FILENO, "\033[D", 3);
	}
	data->cursor_pos--;
	return (0);
}

int	move_cursor_right(t_line_editor *data)
{
	if (data->cursor_pos >= data->line.length)
		return (0);
	if (data->cursor.col >= data->screen.col - 1)
	{
		write(STDOUT_FILENO, "\033[B", 3);
		write(STDOUT_FILENO, "\r", 1);
		data->cursor.col += 1;
		data->cursor.row = 0;
	}
	else
	{
		data->cursor.row += 1;
		write(STDOUT_FILENO, "\033[C", 3);
	}
	data->cursor_pos++;
	return (0);
}

int	stt_history_prev(t_line_editor *data, t_hst *hst)
{
	char	temp[FT_LINE_MAX];
	size_t	len;

	if (hst->count == 0)
		return (0);
	if (data->hst_current == hst->count)
		data->hst_current = hst->count - 1;
	else if (data->hst_current > 0)
		data->hst_current--;
	else
		return (0);
	len = hst_read(data->hst_current, temp, hst);
	if (len == SIZE_MAX)
		return (0);
	// Origin
	ft_memcpy(data->line.ptr, temp, len);
	data->line.length = len;
	data->line.ptr[len] = '\0';
	data->cursor_pos = len;
	redraw_line(data);
	return (0);
}

int	stt_history_next(t_line_editor *data, t_hst *hst)
{
	char	temp[FT_LINE_MAX];
	size_t	len;

	if (hst->count == 0 || data->hst_current >= hst->count)
		return (0);
	data->hst_current++;
	// Origin
	if (data->hst_current >= hst->count)
	{
		data->line.length = 0;
		data->line.ptr[0] = '\0';
		data->cursor_pos = 0;
		redraw_line(data);
		return (0);
	}
	len = hst_read(data->hst_current, temp, hst);
	if (len == SIZE_MAX)
		return (0);
	ft_memcpy(data->line.ptr, temp, len);
	data->line.length = len;
	data->line.ptr[len] = '\0';
	data->cursor_pos = len;
	redraw_line(data);
	return (0);
}

int	handle_arrows(t_line_editor *data, t_hst *hst)
{
	char	seq[2];

	if (read_key(seq) != 1)
		return (0);
	if (seq[0] != '[')
		return (0);
	if (read_key(seq + 1) != 1)
		return (0);
	if (seq[1] == 'A')
		return (stt_history_prev(data, hst));
	else if (seq[1] == 'B')
		return (stt_history_next(data, hst));
	else if (seq[1] == 'C')
		return (move_cursor_right(data));
	else if (seq[1] == 'D')
		return (move_cursor_left(data));
	return (0);
}