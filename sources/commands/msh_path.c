/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/26 17:03:13 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

// CWD changes through chdir, which you acquire from getcwd
// Might be a good idea to copy to a buffer then to envp
// God knows what happens inside getcwd
int	msh_cd(int argc, const char **argv, char **envp)
{
	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	if (getcwd(envp[1], FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	chdir(argv[1]);
	if (getcwd(envp[0], FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	return (0);
}

int	msh_cd2(int argc, const char **argv, t_env *env)
{
	char	buffer[FT_PATH_MAX];

	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	ft_memcpy(env->ptr[1], buffer, ft_strlen(buffer) + 1);
	chdir(argv[1]);
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	ft_memcpy(env->ptr[0], buffer, ft_strlen(buffer) + 1);
	return (0);
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
