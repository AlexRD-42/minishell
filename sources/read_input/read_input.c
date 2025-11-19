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

static int	\
stt_init_line_editor(t_line_editor *data, char *buffer, t_hst *hist)
{
	ft_memset(data, 0, sizeof(t_line_editor));
	data->line.ptr = buffer;
	data->line.length = 0;
	data->cursor_pos = 0;
	data->prompt.ptr = PROMPT;
	data->prompt.length = ft_strlen(PROMPT);
	data->history = hist;
	data->hist_current = hist->count;
	if (get_window_size(&data->screen_rows, &data->screen_cols) == -1)
		return (-1);
	return (0);
}

static int	stt_enable_raw_mode(t_line_editor *data)
{
	struct termios	raw_mode;

	if (tcgetattr(STDIN_FILENO, &data->old_terminal) == -1)
		return (perror("msh: tcgetattr"), -1);
	raw_mode = data->old_terminal;
	raw_mode.c_lflag &= ~(ICANON | ECHO);
	raw_mode.c_cc[VMIN] = 1;
	raw_mode.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode) == -1)
		return (perror("msh: tcsetattr"), -1);
	return (0);
}

static int	stt_disable_raw_mode(t_line_editor *data)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &data->old_terminal) == -1)
		return (perror("msh: tcsetattr"), -1);
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
		return (perror("msh: read"), -1);
	}
	if (ret == 0)
		return (0);
	return (1);
}

int	read_input(char	*buffer, t_hst *hist)
{
	t_line_editor	data;
	char			c;
	ssize_t			ret;

	if (stt_init_line_editor(&data, buffer, hist) == -1)
		return (-1);
	if (stt_enable_raw_mode(&data) == -1)
		return (-1);
	while (1)
	{
		ret = read_key(&c);
		if (g_signal & FT_SIGWINCH)
			if (get_window_size(&data.screen_rows, &data.screen_cols) == -1)
				return (ft_memset(data.line.ptr, 0, FT_LINE_MAX), -1);
		if (g_signal & FT_SIGINT)
			return (rd_handle_sigint(&data));
		if (ret == -1)
			return (ft_memset(data.line.ptr, 0, FT_LINE_MAX), -1);
		if (process_key(&data, c) != 0)
			break ;
	}
	if (stt_disable_raw_mode(&data) == -1)
		return (ft_memset(data.line.ptr, 0, FT_LINE_MAX), -1);
	return (data.line.length);
}
