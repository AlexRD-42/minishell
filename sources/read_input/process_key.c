/* ************************************************************************** */
/* process_key.c                                      :+:      :+:    :+:   */
/* ************************************************************************** */

#include "read_input.h"
#include "unistd.h"

int	stt_finalize_line(t_line_editor *data)
{
	size_t total_len = data->prompt.length + data->line.length;
	int end_row = total_len / data->screen_cols;
	int cur_row = (data->prompt.length + data->cursor_pos) / data->screen_cols;
	
	while (cur_row < end_row)
	{
		write(STDOUT_FILENO, "\033[B", 3);
		cur_row++;
	}
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
		if ((data->prompt.length + data->cursor_pos) % data->screen_cols == 0)
			write(STDOUT_FILENO, "\n\r", 2);
		update_cursor_position(data);
		return (0);
	}
	stt_cursor_home(data);
	i = data->line.length;
	while (i > data->cursor_pos)
	{
		data->line.ptr[i] = data->line.ptr[i - 1];
		i--;
	}
	data->line.ptr[data->cursor_pos] = c;
	data->line.length++;
	data->cursor_pos++;
	redraw_line(data);
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
		data->line.ptr[data->line.length] = '\0';
		move_cursor_left(data);
		write(STDOUT_FILENO, " \b", 2);
		return (0);
	}
	stt_cursor_home(data);
	i = data->cursor_pos - 1;
	while (i < data->line.length - 1)
	{
		data->line.ptr[i] = data->line.ptr[i + 1];
		i++;
	}
	data->line.length--;
	data->cursor_pos--;
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
		data->line.length = 0;	// -1?
		return (-1);
	}
	return (0);
}