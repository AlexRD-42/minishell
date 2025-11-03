/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/03 15:25:03 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

// CWD changes through chdir, which you acquire from getcwd
// Might be a good idea to copy to a buffer then to envp
// God knows what happens inside getcwd
int	msh_cd(int argc, const char **argv, t_env *env)
{
	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	if (getcwd(env->ptr[1], FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	chdir(argv[1]);
	if (getcwd(env->ptr[0], FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	return (0);
}

/* pwd - prints the name of current/working directory
uses getcwd to determine the current path
Using getcwd might introduce mallocs to the code
To do: error handling (stt function maybe)
Check if cwd returns nl
Check short writes */
int	msh_pwd(int argc, const char **argv, t_env *env)
{
	char	buffer[FT_PATH_MAX];

	(void) argc;
	(void) argv;
	(void) env;
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	if (ft_write(STDOUT_FILENO, buffer, ft_strlen(buffer)) < 0)
	{
		perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
