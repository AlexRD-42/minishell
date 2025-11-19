/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 21:35:15 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/19 19:57:57 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

# include <termios.h>
# include "msh_types.h"

# ifndef FT_LINE_MAX
#  define FT_LINE_MAX 4096
# endif

# define PROMPT "msh: "

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
	t_pos	cursor;
	size_t	cursor_pos;
	size_t	hst_current;
}	t_line_editor;

	// int		last_display_lines;
	// int		prompt_start_y;
	// int		prompt_start_x;
void	signal_handler(int sig);
int		setup_signals(void);

char	read_key(char *c);
int		rd_handle_sigint(t_line_editor *data);
int		get_window_size(int *rows, int *cols);
int		process_key(t_line_editor *data, char c);
int		handle_arrows(t_line_editor *data, t_hst *hst);
void	redraw_line(t_line_editor *data);

#endif