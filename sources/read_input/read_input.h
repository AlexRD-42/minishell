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

# include <termios.h>
# include <stdint.h>
# include <stdlib.h>

//# include "msh_types.h"

# ifndef FT_LINE_MAX
#  define FT_LINE_MAX 4096
# endif

typedef struct s_hst_entry
{
	uint32_t start;   // offset into buf
	uint32_t length;  // length in bytes (not including '\0')
}	t_hst_entry;

typedef struct s_hst
{
	size_t		free;		// free space in bytes
	size_t		head;		// next write position
	size_t		first;		// index of oldest entry
	size_t		count;		// number of valid entries
	char		*data;
	t_hst_entry *entries;
}	t_hst;

# define PROMPT "msh: "

typedef struct s_str
{
	char	*ptr;
	size_t	length;
}	t_str;

typedef struct s_kstr
{
	const char	*ptr;
	size_t		length;
}	t_kstr;

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
	t_hst	*hist;
}	t_line_editor;

extern volatile __sig_atomic_t g_signal;

void	redraw_line(t_line_editor *data);
void	cursor_end(t_line_editor *data);
void	cursor_home(t_line_editor *data);
int		move_cursor_left(t_line_editor *data);
int		move_cursor_right(t_line_editor *data);
int		stt_history_prev(t_line_editor *data);
int		stt_history_next(t_line_editor *data);
int		handle_arrows(t_line_editor *data);
int		stt_finalize_line(t_line_editor *data);
int		stt_handle_printable_char(t_line_editor *data, char c);
int		stt_handle_backspace(t_line_editor *data);
int		process_key(t_line_editor *data, char c);
int		get_window_size(t_pos *coords);
int		rd_handle_sigint(t_line_editor *data);
int		stt_init_read(char *buffer, t_hst *hst);
int		read_input(char	*buffer, t_line_editor *data);
char	read_key(char *c);

//to test
size_t	ft_strlen(const char *s);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
int	ft_memcmp(const void *s1, const void *s2, size_t n);
size_t	hst_add_entry(const char *str, size_t length, t_hst *hst);
size_t	hst_read(size_t index, char *buffer, const t_hst *hst);
void setup_signals(void);
void handle_sigwinch(int sig);
void handle_sigint(int sig);

#endif