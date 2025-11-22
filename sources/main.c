/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:25:47 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 21:32:25 by feazeved         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include "msh_utils.h"

volatile sig_atomic_t g_signal;

void	signal_handler(int sig)
{
	g_signal = sig;
}

static
int	stt_notty_mode(t_env *env, t_hst *hst)
{
	// const char	*str;
	// size_t		token_count;

	// str = argv[1];	// We only care about the first argument
	// token_count = tokenize(msh, str);
	// if (token_count == SIZE_MAX)
	// {

	// }
	// syntax error;
	// Execute
	// Exit
	return (0);
}

static
int	stt_tty_mode(t_env *env, t_hst *hst)
{
	t_token	tokens[FT_TOKEN_COUNT];	// Allocation
	char	line[FT_LINE_MAX];
	size_t	len;
	int		rvalue;

	while (1)
	{
		len = init_read(line, hst); // Read char by char in non canonical mode
		if (len == SIZE_MAX) // Control + D on empty line -> exit
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (len == 0) // Empty string "" , do not add to history
			continue;
		hst_add_entry(line, len, hst);
		if (parsing(tokens, line, env) == SIZE_MAX)
		{
			// att_exit(2, true); //exit_status for syntax error: 2
			// continue ;
		}
		rvalue = exec_line(tokens, tokens + FT_TOKEN_COUNT, env);
	}
	return (rvalue); // When passing false, only returns current exit_status
}

int	main(int argc, const char **argv, const char **envp)
{
	static t_env	env;
	static t_hst	hst;
	int				rvalue;

	(void)(argv && argc);
	rvalue = 0;
	if (msh_init(&env, &hst, envp) == 0)
	{
		if (isatty(STDIN_FILENO) == 1)
			rvalue = stt_tty_mode(&env, &hst);
		else
			rvalue = stt_notty_mode(&env, &hst);
	}
	return (rvalue);
}
