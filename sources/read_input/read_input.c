/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 16:52:03 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/18 16:52:18 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "read_input.h"
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdbool.h>

static int	\
stt_init_line_editor(t_line_editor *data, char *buffer, t_hst *hst, struct termios raw_mode)
{
	raw_mode.c_lflag &= ~((tcflag_t)(ICANON | ECHO));
	raw_mode.c_cc[VMIN] = 1;
	raw_mode.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode) == -1)
		return (-1);
	ft_memset(data, 0, sizeof(t_line_editor));
	if (get_window_size(&data->screen) == -1)
		return (-2);
	data->line.ptr = buffer;
	data->line.length = 0;
	data->cursor_pos = 0;
	data->prompt.ptr = PROMPT;
	data->prompt.length = ft_strlen(PROMPT);
	data->cursor.col = data->prompt.length;
	data->hist = hst;
	data->hst_current = hst->count;
	return (0);
}

// Return: 1) OK,  0) EOF/singal,  -1) fatal error
char	read_key(char *c)
{
	ssize_t	ret;

	ret = read(STDIN_FILENO, c, 1);
	if (ret == -1)
	{
		if (errno == EINTR)
			return (0);
		return (-1);
	}
	if (ret == 0)
		return (0);
	return (1);
}

int	read_input(char	*buffer, t_line_editor *data)
{
	char	c;
	ssize_t	ret;

	write(STDOUT_FILENO, data->prompt.ptr, data->prompt.length);
	while (true)
	{
		ret = read(STDIN_FILENO, &c, 1);
		if (ret == -1 && errno != EINTR)
			return (-1);
		if (g_signal == SIGWINCH)
			if (get_window_size(&data->screen) == -1)
				return (-1);
		if (g_signal == SIGINT)
			return (rd_handle_sigint(data));
		if (process_key(data, c) != 0)
			break ;
	}
	return (data->line.length);
}

int	stt_init_read(char *buffer, t_hst *hst)
{
	int				rvalue;
	t_line_editor	data;
	struct termios	old_mode;

	if (tcgetattr(STDIN_FILENO, &old_mode) == -1)
		return (-1);
	rvalue = 0;
	if (stt_init_line_editor(&data, buffer, hst, old_mode) == 0)
	{
		rvalue = read_input(buffer, &data);
	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_mode);
	return (rvalue);
}
