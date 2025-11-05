/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/31 12:23:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

void	non_interactive_shell(t_shell *shell)
{

}
// 1. Display prompt
// 2. Read user input into shell->input
// 3. Dont continue if shell->input is empty
// 4.1 Add input to history
// 4.2 tokenize input
// 4.3 Parse tokens
// 4.4 Execute
// 4.5? Clean struct
void	interactive_shell(t_shell *shell)
{
	while (1)
	{
		ft_write(STDOUT_FILENO, PROMPT, ft_strlen(PROMPT));
		if (read_intput(shell) == -1)
			break ;
		if (!shell->input[0])
			continue ;
		/* add to history*/
		if (tokenize(shell) == -1)
			continue ;
		execute(shell);
	}
}
// To check errors from isatty() AND to check if we need non_interactive_shell
// isatty returns 1 if the passed FD is an open fd referring to a terminal;
void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	shell->envp = envp;
	if (isatty(STDIN_FILENO))
		shell->non_interactive = false;
	else
	{
		shell->non_interactive = true;
		if (argc > 1)
			shell->input = argv;
		else
			shell->input = NULL; 
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	init_shell(&shell, argc, argv, envp);
	if (shell.non_interactive)
		non_interactive_shell(&shell);
	else
		interactive_shell(&shell);
	exit(0);
}