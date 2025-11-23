/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 22:00:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_utils.h"

// Remember the prefix PWD= (check if needed)
// Review: Empty arguments
int	msh_cd(t_vecp *argv, t_env *env)
{
	char	buffer[FT_PATH_SIZE * 2];

	if (argv->count < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	if (argv->count > 2)
		return (ft_error("msh_cd: too many arguments", "", 1));
	ft_memcpy(buffer, "OLDPWD=", 8);
	if (getcwd(buffer + 7, FT_PATH_SIZE) == NULL)
		return (ft_error("msh_getcwd: ", NULL, 1));
	if (env_export(buffer, env))
		return (ft_error("msh_export: out of memory", "", 1));
	if (chdir(argv->ptr[1]))
		return (ft_error("msh_chdir: ", NULL, 1));
	if (getcwd(buffer + 7, FT_PATH_SIZE) == NULL)
		return (ft_error("msh_getcwd: ", NULL, 1));
	if (env_export(buffer + 3, env))
		return (ft_error("msh_export: out of memory", "", 1));
	return (0);
}
