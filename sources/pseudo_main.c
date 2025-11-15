/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pseudo_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/11 15:47:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

#define PROMPT "msh: "

void	non_interactive_shell(t_shell *shell, t_token *tokens)
{
	
}

void	print_prompt(void)
{
	write(1, PROMPT, ft_strlen(PROMPT));
}

void	interactive_shell(t_shell *shell)
{
	char	line[FT_PAGE_SIZE];
	ssize_t	n;

	setup_signals();
	while (1)
	{
		print_prompt();
		setup_terminal();
		n = read_input(shell->history, &line, sizeof(line));
		restore_terminal_config();
		if (n == -1)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (!line[0])
			continue ;
		hst_add_entry(line, n, &shell->history);
		if (tokenize(&shell, line) == -1)
			continue ;
		execute(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argc;
	ft_memset(&shell, 0, sizeof(shell));
	env_init(&shell.env, envp);
	//shlvl();
	if (isatty(STDIN_FILENO))
		interactive_shell(&shell);
	else
		non_interactive_shell(&shell, NULL);
	exit(0);
}