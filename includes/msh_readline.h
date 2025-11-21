/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_readline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 21:35:15 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/21 12:27:00 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

# include "msh_types.h"

typedef struct s_pos
{
	uint16_t	row;
	uint16_t	col;
}	t_pos;

typedef struct s_line_editor
{
	t_str	line;
	t_kstr	prompt;
	t_pos	screen;
	int		input_lines;
	size_t	cursor_pos;
	t_pos	cursor;
	size_t	hst_current;
	t_hst	*hst;
}	t_line_editor;

char	read_key(char *c);
int		process_key(t_line_editor *data, char c);
int		move_cursor_left(t_line_editor *data);
int		move_cursor_right(t_line_editor *data);
int		handle_arrows(t_line_editor *data);
void	redraw_line(t_line_editor *data);
void	cursor_end(t_line_editor *data);
void	cursor_home(t_line_editor *data);
int		get_window_size(t_pos *coords);
int		rd_handle_sigint(t_line_editor *data);
void	reset_line(t_line_editor *data);

#endif
