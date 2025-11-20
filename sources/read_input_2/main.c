/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:59:46 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 10:01:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>
#include "minishell.h"
#include "msh_utils.h"
#include "read_input.h"

volatile sig_atomic_t g_signal;

// 0) Ok, -1) Raw mode was not set, -2) Get Window Size failed
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
	data->hst_current = hst->count;
	return (0);
}

// Always restores terminal mode
static
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
		rvalue = read_input(buffer, hst, data);
	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_mode);
	return (rvalue);
}

int main(void)
{
	char		data[FT_HST_SIZE];
	t_hst_entry	data_ptr[FT_HST_COUNT];
	t_hst		hst = {FT_HST_SIZE, 0, 0, 0, data, data_ptr};
	char		line_buffer[FT_LINE_MAX];
	int			len;

	setup_signals();
	write(STDOUT_FILENO, "Minishell Test\n", 15);
	write(STDOUT_FILENO, "Digite 'exit' para sair\n\n", 26);

	while (1)
	{
		len = stt_init_read(line_buffer, &hst);
		
		if (len == -1)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		if (len == 0)
			continue;
		
		if (ft_strncmp(line_buffer, "exit", 4) == 0)
			break;
		
		hst_add_entry(line_buffer, len, &hst);
		
		write(STDOUT_FILENO, "Você digitou: ", 15);
		write(STDOUT_FILENO, line_buffer, len);
		write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}
