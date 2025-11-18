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

int	get_window_size(int *rows, int *cols)
{
	struct winsize	ws;

	if (g_signal & FT_SIGWINCH)
		g_signal &= ~FT_SIGWINCH;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (perror("msh: ioctl"), -1);
	*cols = ws.ws_col;
	*rows = ws.ws_row;
	return (0);
}

int	rd_handle_sigint(t_line_editor *data)
{
	write(STDOUT_FILENO, "^C\n", 3);
	data->line.ptr[0] = '\0';
	g_signal &= ~FT_SIGINT;
	data->line.length = 0;
	return (0);
}
