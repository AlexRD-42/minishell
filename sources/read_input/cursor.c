/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 20:54:06 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/18 20:54:17 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "read_input.h"

int	stt_ft_itoa_stack(int64_t number, char *buf)
{
	const int8_t	sign = (number >= 0) - (number < 0);
	char			temp[32];
	int				i;
	int				j;

	number *= sign;
	i = 0;
	if (number < 0)
		number = -number;
	if (number == 0)
		temp[i++] = '0';
	while (number > 0)
	{
		temp[i++] = (number % 10) + '0';
		number /= 10;
	}
	if (sign == -1)
		temp[i++] = '-';
	j = 0;
	while (i > 0)
		buf[j++] = temp[--i];
	buf[j] = '\0';
	return (j);
}

void	\
move_cursor_to_position(t_line_editor *data, int target_y, int target_x)
{
	char	buf_y[12];
	char	buf_x[12];
	int		len_y;
	int		len_x;

	len_y = stt_ft_itoa_stack(target_y + 1, buf_y);
	len_x = stt_ft_itoa_stack(target_x + 1, buf_x);
	write(STDOUT_FILENO, "\033[", 2);
	write(STDOUT_FILENO, buf_y, len_y);
	write(STDOUT_FILENO, ";", 1);
	write(STDOUT_FILENO, buf_x, len_x);
	write(STDOUT_FILENO, "H", 1);
}

void	update_cursor_position(t_line_editor *data)
{
	size_t	total_pos;

	total_pos = data->prompt.length + data->cursor_pos;
	data->cursor_y = total_pos / data->screen_cols;
	data->cursor_x = total_pos % data->screen_cols;
}

void	redraw_from_cursor(t_line_editor *data)
{
	size_t	chars_after;
	size_t	i;

	chars_after = data->line.length - data->cursor_pos;
	write(STDOUT_FILENO, data->line.kptr + data->cursor_pos, chars_after);
	write(STDOUT_FILENO, " ", 1);
	i = 0;
	while (i <= chars_after)
	{
		write(STDOUT_FILENO, "\b", 1);
		i++;
	}
}

void	redraw_line(t_line_editor *data)
{
	int	i;

	write(STDOUT_FILENO, "\r\033[J", 4);
	write(STDOUT_FILENO, data->prompt.kptr, data->prompt.length);
	write(STDOUT_FILENO, data->line.kptr, data->line.length);
	update_cursor_position(data);
	write(STDOUT_FILENO, "\r", 1);
	i = 0;
	while (i < data->prompt.length + data->cursor_pos)
    {
        write(STDOUT_FILENO, "\033[C", 3);
        i++;
    }
	update_cursor_position(data);
}
