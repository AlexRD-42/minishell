/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <feazeved@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 21:35:15 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/15 21:35:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

# include "../../includes/minishell.h"
# include <termios.h>

# ifndef FT_SIGWINCH
#  define FT_SIGWINCH 1
# endif

# ifndef FT_SIGINT
#  define FT_SIGINT 2
# endif

# ifndef FT_LINE_MAX
#  define FT_LINE_MAX 4096
# endif

# define PROMPT "msh: "

extern int	g_signal;

typedef struct s_line_editor
{
	struct termios	old_terminal;
	t_str			line;
	t_str			prompt;
	int				screen_rows;
	int				screen_cols;
	int				cursor_x;
	int				cursor_y;
	size_t			cursor_pos;
	size_t			hist_current;
	t_hst			*history;
}	t_line_editor;

int		read_input(char	*buffer, t_hst *hist);
char	read_key(char *c);
int		rd_handle_sigint(t_line_editor *data);
int		get_window_size(int *rows, int *cols);
int		process_key(t_line_editor *data, char c);
int		handle_arrows(t_line_editor *data);
void	update_cursor_position(t_line_editor *data);
void	redraw_line(t_line_editor *data);
void	redraw_from_cursor(t_line_editor *data);
int		move_cursor_right(t_line_editor *data);
int		move_cursor_left(t_line_editor *data);
size_t	hst_read(size_t index, char *buffer, const t_hst *hst);

#endif