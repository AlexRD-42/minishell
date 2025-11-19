/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interrupts.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 17:37:38 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 19:23:33 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "minishell.h"
#include "msh_utils.h"
#include "read_input.h"

extern volatile sig_atomic_t g_signal; 

void	signal_handler(int sig)
{
	g_signal = sig;
}

// All processes have default actions for signals received
// You can override this action by registering a handler (registering SIG_IGN)
int	setup_signals(void)
{
	struct sigaction	sigmain;

	ft_memset(&sigmain, 0, sizeof(sigmain));
	sigemptyset(&sigmain.sa_mask);			// Clears the mask
	sigmain.sa_flags = 0;
	sigmain.sa_handler = signal_handler;			// Registers a handler
	if (sigaction(SIGWINCH, &sigmain, NULL) < 0)	// Now listening to SIGWINCH
		return (ft_error("msh_signals: ", strerror(errno), -1));
	if (sigaction(SIGINT, &sigmain, NULL) < 0)
		return (ft_error("msh_signals: ", strerror(errno), -1));
	if (sigaction(SIGQUIT, &sigmain, NULL) < 0)
		return (ft_error("msh_signals: ", strerror(errno), -1));
	return (0);
}

int	get_window_size(int *rows, int *cols)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
		return (ft_error("msh: ioctl", strerror(errno), -1));
	g_signal = 0;
	*cols = ws.ws_col;
	*rows = ws.ws_row;
	return (0);
}

int	rd_handle_sigint(t_line_editor *data)
{
	write(STDOUT_FILENO, "^C\n", 3);
	data->line.ptr[0] = '\0';
	g_signal = 0;
	data->line.length = 0;
	return (0);
}