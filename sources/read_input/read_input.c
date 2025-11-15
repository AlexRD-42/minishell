/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:25:17 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/14 19:14:54 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <errno.h>
#include <sys/ioctl.h>
#include <stdbool.h>

#include "read_input.h"

int	get_terminal_width(void)
{
	struct winsize	terminal;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) == -1)
		return (80);
	return (terminal.ws_col);
}

void redraw_from_cursor(t_line *line, const char *prompt)
{
    int saved_cursor;
    int saved_row;
    int final_row;
    
    saved_cursor = line->cursor;
    saved_row = line->visual_row;
    write(STDOUT_FILENO, line->buffer + line->cursor, line->len - line->cursor);
    write(STDOUT_FILENO, " ", 1);
    line->cursor = line->len;
    set_visual_pos(line);
    final_row = line->visual_row;
    line->cursor = saved_cursor;
    set_visual_pos(line);
    move_cursor(saved_row, line->visual_col, final_row);
}

void	set_visual_pos(t_line *line)
{
	size_t	total_pos;

	total_pos = line->prompt_len + line->cursor;
	line->visual_row = total_pos / line->term_width;
	line->visual_col = total_pos % line->term_width;
}

int calculate_total_lines(t_line *line)
{
    size_t total_len;
    
    total_len = line->prompt_len + line->len;
    return ((total_len / line->term_width) + 1);
}

void	move_cursor(int row, int col, int current_row)
{
	int		diff;
	int		n;

	diff = row - current_row;
	while (diff-- > 0)
		write(STDOUT_FILENO, "\033[B", 3);
	while (diff++ < 0)
		write(STDOUT_FILENO, "\033[A", 3);
	write(STDOUT_FILENO, "\r", 1);
	while (col-- > 0)
		write(STDOUT_FILENO, "\033[C", 3);
}

int	move_cursor_left(t_line *line)
{
	int	old_row;

	if (line->cursor == 0)
		return (0);
	old_row = line->visual_row;
	line->cursor--;
	set_visual_pos(line);
	if (line->visual_row != old_row)
		write(STDOUT_FILENO, "\033[A", 3);
	else
		write(STDOUT_FILENO, "\033[D", 3);
	return (0);
}

static int move_cursor_right(t_line *line)
{
    int old_row;
    
    if (line->cursor >= line->len)
        return (0);
    old_row = line->visual_row;
    line->cursor++;
    set_visual_pos(line);
    if (line->visual_row != old_row)
	{
        write(STDOUT_FILENO, "\033[B", 3);
    	write(STDOUT_FILENO, "\r", 1);
	}
    else
        write(STDOUT_FILENO, "\033[C", 3);
    return (0);
}

void	clear_all_lines(t_line *line)
{
	int	total_lines;
	int	current_row;
	int	i;

	current_row = line->visual_row;
	total_lines = calculate_total_lines(line);
	while (current_row < total_lines - 1)
		write(STDOUT_FILENO, "\033[B", 3);
	i = 0;
	while (i < total_lines)
	{
		write(STDOUT_FILENO, "\r\033[K", 4);
		if (i < total_lines - 1)
			write(STDOUT_FILENO, "\033[A", 3);
		i++;
	}
	write(STDOUT_FILENO, "\r", 1);
}

void	draw_line(t_line *line, const char *prompt)
{
	int	old_row;
	int	new_row;

	old_row = line->visual_row;
	clear_all_lines(line);
	write(STDOUT_FILENO, prompt, line->prompt_len);
	write(STDOUT_FILENO, line->buffer, line->len);
	set_visual_pos(line);
	new_row = calculate_total_lines(line) - 1;
	move_cursor(line->visual_row, line->visual_col, new_row);
}

static void	\
stt_handle_control_c(t_line *line, char **buffer, size_t buffer_size)
{
	ft_memset(*buffer, 0, buffer_size);
	g_signal = 0;
	write(STDOUT_FILENO, PROMPT, line->prompt_len);
	line->len = 0;
	line->cursor = 0;
	set_visual_pos(line);
}

void	handle_resize(t_line *line, char *prompt)
{
	int	total_lines;

	line->term_width = get_terminal_width();
	write(STDOUT_FILENO, "\033[2J\033[H", 7);
	write(STDOUT_FILENO, prompt, line->prompt_len);
    write(STDOUT_FILENO, line->buffer, line->len);
	set_visual_pos(line);
	total_lines = calculate_total_lines(line);
	move_cursor(line->visual_row, line->visual_col, total_lines - 1);
	g_signal = 0;
}

int finalize_line(t_line *line)
{
    int total_lines;
    int final_row;
    
    total_lines = calculate_total_lines(line);
    final_row = total_lines - 1;
    move_cursor(final_row, line->term_width - 1, line->visual_row);
    write(STDOUT_FILENO, "\n", 1);
    line->buffer[line->len] = '\0';
    return (line->len);
}

int handle_backspace(t_line *line, const char *prompt)
{
    size_t i;
    
    if (line->cursor == 0)
        return (0);
    i = line->cursor - 1;
    while (i < line->len - 1)
    {
        line->buffer[i] = line->buffer[i + 1];
        i++;
    }
    line->len--;
    line->cursor--;
    move_cursor_left(line);
    redraw_from_cursor(line, prompt);
    return (0);
}

void insert_char_at_cursor(t_line *line, char c, const char *prompt)
{
    size_t i;

    i = line->len;
    while (i > line->cursor)
    {
        line->buffer[i] = line->buffer[i - 1];
        i--;
    }
    line->buffer[line->cursor] = c;
    line->len++;
    line->cursor++;
    redraw_from_cursor(line, prompt);
}

int	history_prev(t_line *line, t_hst *hist, const char *prompt)
{
	char	temp[4096];
	size_t	len;

	if (hist->count == 0)
		return (0);
	if (hist->current == 0)
		hist->current = hist->count;
	hist->current--;
	len = hst_read(hist->current, temp, hist);
	if (len == SIZE_MAX)
		return (0);
	clear_all_lines(line);
	write(STDOUT_FILENO, prompt, line->prompt_len);
	write(STDOUT_FILENO, temp, len);
	line->len = len;
	ft_memcpy(line->buffer, temp, len);
	line->buffer[len] = '\0';
	line->cursor = len;
	set_visual_pos(line);
	return (0);
}

int	history_next(t_line *line, t_hst *hist, const char *prompt)
{
	char	temp[4096];
	size_t	len;

	if (hist->count == 0)
		return (0);
	hist->current++;
	if (hist->current >= hist->count)
	{
		hist->current = hist->count;
		clear_all_lines(line);
		write(STDOUT_FILENO, prompt, line->prompt_len);
		line->len = 0;
		line->cursor = 0;
		line->buffer[0] = '\0';
		set_visual_pos(line);
		return (0);
	}
	len = hst_read(hist->current, temp, hist);
	if (len == SIZE_MAX)
		return (0);
	clear_all_lines(line);
	write(STDOUT_FILENO, prompt, line->prompt_len);
	write(STDOUT_FILENO, temp, len);
	line->len = len;
	ft_memcpy(line->buffer, temp, len);
	line->buffer[len] = '\0';
	line->cursor = len;
	set_visual_pos(line);
	return (0);
}

int handle_arrows(t_line *line, t_hst *hist, const char *prompt)
{
    char c;
    ssize_t	ret;

    ret = read(STDIN_FILENO, &c, 1);
	if (ret <= 0)
        return (handle_read_error(line, line->buffer, 4096));
    if (c != '[')
        return (0);
	ret = read(STDIN_FILENO, &c, 1);
	if (ret <= 0)
        return (handle_read_error(line, line->buffer, 4096));
    if (c == 'A')
        return (history_prev(line, hist, prompt));
    else if (c == 'B')
        return (history_next(line, hist, prompt));
    else if (c == 'C')
        return (move_cursor_right(line));
    else if (c == 'D')
        return (move_cursor_left(line));
    return (0);
}

int	handle_read_error(t_line *line, char *buffer, size_t buffer_size)
{
	if (g_signal & FT_SIGINT && errno == EINTR)
	{
		ft_memset(buffer, 0, buffer_size);
		g_signal = 0;
		return (0);
	}
	return (-1);
}

int read_input(t_hst *history, char *buffer[4096], size_t buffer_size)
{
	t_line  line;
    char    c;
    ssize_t ret;
    
    ft_memset(&line, 0, sizeof(t_line));
    line.buffer = *buffer;
    line.term_width = get_terminal_width();
    line.prompt_len = ft_strlen(PROMPT);
	while (1)
	{
		if (g_signal & FT_SIGWINCH)
			handle_resize(&line, PROMPT);
		ret = read(STDIN_FILENO, &c, 1);
		if (ret <= 0)
			return (handle_read_error(&line, line.buffer, buffer_size));
		if (c == '\n')
			return (finalize_line(&line));
		else if (c == 4 && line.len == 0)
			return (-1);
		else if (c == 127 || c == 8)
			handle_backspace(&line, PROMPT);
		else if (c == 27)
		{
			ret = handle_arrows(&line, history, PROMPT);
			if (ret == -1)
				return (-1);
			continue ;
		}
		if (c >= 32 && c < 127 && line.len < buffer_size - 1)
        {
            if (line.cursor == line.len)
            {
                line.buffer[line.len++] = c;
                line.cursor++;
                write(STDOUT_FILENO, &c, 1);
                set_visual_pos(&line);
            }
            else
                insert_char_at_cursor(&line, c, PROMPT);
        }
	}
}
