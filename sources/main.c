/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:25:47 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 22:02:54 by adeimlin         ###   ########.fr       */
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
	// t_token	tokens[FT_TOKEN_COUNT];
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
	t_token	tokens[FT_TOKEN_COUNT];
	char	line[FT_LINE_MAX];
	size_t	len;
	int		rvalue;
	t_token	*end;

	rvalue = env->exit_status;
	while (1)
	{
		len = init_read(line, hst, env);
		if (len == SIZE_MAX)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (len == 0)
			continue;
		hst_add_entry(line, len, hst); // Review error
		if (parsing(tokens, line, &end, env) == SIZE_MAX)
		{
			env->exit_status = 2;
			continue ;
		}
		if (tokens[0].type != E_END)
			rvalue = exec_line(tokens, end, env);
	}
	return (rvalue);
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
