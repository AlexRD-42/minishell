/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_arrows.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:33:52 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 12:27:08 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "msh_readline.h"
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

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
	t_hst	*hst;
	char	temp[FT_LINE_MAX];
	size_t	len;

	hst = data->hst;
	if (hst->count == 0 || data->hst_current == 0)
		return (0);
	cursor_home(data);
	write(STDOUT_FILENO, "\033[0J", 4);
	data->hst_current--;
	len = hst_read(data->hst_current, temp, hst);
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
	t_hst	*hst;
	char	temp[FT_LINE_MAX];
	size_t	len;

	hst = data->hst;
	if (hst->count == 0 || data->hst_current >= hst->count)
		return (0);
	cursor_home(data);
	write(STDOUT_FILENO, "\033[0J", 4);
	data->hst_current++;
	if (data->hst_current >= hst->count)
		return (reset_line(data), 0);
	len = hst_read(data->hst_current, temp, hst);
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
