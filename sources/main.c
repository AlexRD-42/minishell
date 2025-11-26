/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 12:25:47 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 10:58:31 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include "minishell.h"
#include "msh_utils.h"
#include "msh_defines.h"
#include "msh_types.h"

volatile sig_atomic_t	g_signal;

void	signal_handler(int sig)
{
	g_signal = sig;
}

static
int	stt_notty_mode(t_env *env, char *line)
{
	t_token		tokens[FT_TOKEN_COUNT];
	ssize_t		bytes_read;
	int			rvalue;
	t_token		*end;

	rvalue = 0;
	bytes_read = read(STDIN_FILENO, line, FT_PIPE_SIZE);
	if (bytes_read <= 0)
		return (bytes_read != 0);
	line[bytes_read] = '\0';
	if (parsing(tokens, line, &end, env) == SIZE_MAX)
		return (2);
	if (tokens[0].type != E_END)
		rvalue = exec_line(tokens, end, env);
	return (rvalue);
}

static
int	stt_tty_mode(t_env *env, t_hst *hst, char *line, int rvalue)
{
	t_token		tokens[FT_TOKEN_COUNT];
	size_t		len;
	t_token		*end;

	while (1)
	{
		len = init_read(line, hst, env);
		if (len == SIZE_MAX)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (len == 0)
			continue ;
		hst_add_entry(line, len, hst);
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
	static char		line[FT_LINE_MAX];
	int				rvalue;

	(void)(argv && argc);
	if (msh_init(&env, &hst, envp) != 0)
		return (ft_error("msh_init: failed to initialize", "", 1));
	if (isatty(STDIN_FILENO) == 1)
		rvalue = stt_tty_mode(&env, &hst, line, env.exit_status);
	else
		rvalue = stt_notty_mode(&env, line);
	return (rvalue);
}
