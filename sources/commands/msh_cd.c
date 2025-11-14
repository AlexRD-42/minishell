/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/14 12:20:10 by adeimlin         ###   ########.fr       */
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
	if (getcwd(env->ptr[1], FT_PATH_SIZE) == NULL)
	{
		perror("cwd");
		return (1);
	}
	chdir(argv[1]);
	if (getcwd(env->ptr[0], FT_PATH_SIZE) == NULL)
	{
		perror("cwd");
		return (1);
	}
	return (0);
}
