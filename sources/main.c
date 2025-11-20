/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:25:47 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 11:57:29 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include "minishell.h"
#include "msh_types.h"
#include "msh_utils.h"

volatile sig_atomic_t g_signal; 

void	signal_handler(int sig)
{
	g_signal = sig;
}

static
int	stt_notty_mode(int argc, const char **argv, t_shell *msh)
{
	const char	*str;
	size_t		token_count;

	if (argc < 2)
		return (1);
	str = argv[1];	// We only care about the first argument
	token_count = tokenize(msh, str);
	if (token_count == SIZE_MAX)
		// syntax error;
	// Execute
	// Exit
}

static
int	stt_tty_mode(t_shell *msh)
{
	// Read Input
	// Tokenize
	// Execute
	// Continue unless break
}

int	main(int argc, const char **argv, const char **envp)
{
	t_memory		mem;
	t_shell			msh;
	struct termios	shell_src;
	int				rvalue;

	if (tcgetattr(STDIN_FILENO, &shell_src) == -1)
		return (-1);
	rvalue = 0;
	msh.shell_src = &shell_src;
	if (msh_init(&mem, &msh, envp) == 0)
	{
		if (isatty(STDIN_FILENO) == 1)
			rvalue = stt_tty_mode(&msh);
		else
			rvalue = stt_notty_mode(argc, argv, &msh);
	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &shell_src);	// Need to be extra careful with exits
	return (rvalue);
}
