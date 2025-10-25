/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/25 14:01:29 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

// CWD changes through chdir, which you acquire from getcwd
int	msh_cd(int argc, const char **argv, const char **envp)
{
	
}

/* pwd - prints the name of current/working directory
uses getcwd to determine the current path
Using getcwd might introduce mallocs to the code
To do: error handling (stt function maybe)
Check if cwd returns nl
Check short writes */
int	msh_pwd(int argc, const char **argv)
{
	char	buffer[FT_PATH_MAX];

	(void) argc;
	(void) argv;
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	if (write(STDOUT_FILENO, buffer, ft_strlen(buffer)) < 0)
	{
		perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
