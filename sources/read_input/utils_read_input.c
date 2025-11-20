/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_read_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:14:00 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/18 21:14:15 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "read_input.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int	get_window_size(t_pos *coords)
{
	struct winsize	ws;

	if (g_signal == SIGWINCH)
		g_signal = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (perror("msh: ioctl"), -1);
	coords->col = ws.ws_col;
	coords->row = ws.ws_row;
	return (0);
}

int	rd_handle_sigint(t_line_editor *data)
{
	write(STDOUT_FILENO, "^C\n", 3);
	data->line.ptr[0] = '\0';
	data->line.length = 0;
	data->cursor_pos = 0;
	data->cursor.col = data->prompt.length;
	data->cursor.row = 0;
	data->hst_current = data->hist->count;
	g_signal = 0;
	return (0);
}
