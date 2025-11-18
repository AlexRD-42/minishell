/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 12:43:19 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

// Remember the prefix PWD= (check if needed)
int	msh_cd(t_argv *argv, t_env *env)
{
	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	if (getcwd(env->ptr[E_OLDPWD], FT_PATH_SIZE) == NULL)
	{
		perror("cwd");
		return (1);
	}
	chdir(argv[1]);
	if (getcwd(env->ptr[E_PWD], FT_PATH_SIZE) == NULL)
	{
		perror("cwd");
		return (1);
	}
	return (0);
}
