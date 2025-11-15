/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: feazeved <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 21:35:15 by feazeved          #+#    #+#             */
/*   Updated: 2025/11/15 21:35:39 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READ_INPUT_H
# define READ_INPUT_H

# include "../../includes/minishell.h"
#define PROMPT "msh: "

#define FT_SIGWINCH 1
#define FT_SIGINT 2


extern int g_signal;


typedef struct	s_line
{
	char	*buffer;
	size_t	len;
	size_t	cursor;
	int		visual_row;
	int		visual_col;
	int		term_width;
	size_t	prompt_len;
}	t_line;

int read_input(t_hst *history, char *buffer[4096], size_t buffer_size);
int	handle_read_error(t_line *line, char *buffer, size_t buffer_size);
void redraw_from_cursor(t_line *line, const char *prompt);
int handle_arrows(t_line *line, t_hst *hist, const char *prompt);
int	history_next(t_line *line, t_hst *hist, const char *prompt);
int	history_prev(t_line *line, t_hst *hist, const char *prompt);
void insert_char_at_cursor(t_line *line, char c, const char *prompt);
int handle_backspace(t_line *line, const char *prompt);
int finalize_line(t_line *line);
void	handle_resize(t_line *line, char *prompt);
void	draw_line(t_line *line, const char *prompt);
void	clear_all_lines(t_line *line);
int	move_cursor_left(t_line *line);
void	move_cursor(int row, int col, int current_row);
int calculate_total_lines(t_line *line);
void	set_visual_pos(t_line *line);
int	get_terminal_width(void);
size_t	hst_read(size_t index, char *buffer, const t_hst *hst);

#endif