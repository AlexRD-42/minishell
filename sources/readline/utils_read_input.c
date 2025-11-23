/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_read_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 21:14:00 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 12:25:00 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_readline.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

extern volatile sig_atomic_t	g_signal;

int	get_window_size(t_pos *coords)
{
	struct winsize	ws;

	if (g_signal == SIGWINCH || g_signal == SIGQUIT)
		g_signal = 0;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (perror("msh: ioctl"), -1);
	coords->col = ws.ws_col;
	coords->row = ws.ws_row;
	return (0);
}

void	reset_line(t_line_editor *data)
{
	data->line.ptr[0] = '\0';
	data->line.length = 0;
	data->cursor_pos = 0;
	data->cursor.col = data->prompt.length;
	data->cursor.row = 0;
	data->hst_current = data->hst->count;
}

int	rd_handle_sigint(t_line_editor *data)
{
	write(STDOUT_FILENO, "^C\n", 3);
	reset_line(data);
	g_signal = 0;
	return (0);
}
