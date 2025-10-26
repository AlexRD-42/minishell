/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/26 16:59:45 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

// Running env with no arguments will print the env to stdout
int	msh_env(int argc, const char **argv, const char **envp)
{
	size_t	i;
	size_t	length;
	size_t	offset;
	uint8_t	buffer[FT_ENV_SIZE];

	(void) argc;
	(void) argv;
	i = 0;
	offset = 0;
	while (envp[i] != NULL)
	{
		length = ft_strlen(envp[i]);
		ft_memcpy(buffer + offset, envp[i], length);
		offset += length;
		buffer[offset++] = '\n';
		i++;
	}
	return (ft_write(STDOUT_FILENO, buffer, offset) <= 0);
}

// int	msh_env(int argc, const char **argv, const char **envp)
// {
// 	size_t	i;
// 	size_t	length;

// 	(void) argc;
// 	(void) argv;
// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		length = ft_strlen(envp[i]);
// 		if (ft_write(STDOUT_FILENO, envp[i], length) <= 0)
// 			return (1);
// 		if (ft_write(STDOUT_FILENO, "\n", 1) <= 0)
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// 
int	msh_export(int argc, const char **argv, t_env *env)
{
	int		rvalue;
	size_t	i;

	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	rvalue = 0;
	i = 1;
	while (argv[i] != NULL)
	{
		rvalue += env_add(env, argv[i]) != 0;
		i++;
	}
	return (rvalue);
}

// 0  All name operands were successfully unset
// >0 At least one name could not be unset
int	msh_unset(int argc, const char **argv, t_env *env)
{
	int		rvalue;
	size_t	i;

	if (argc < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	rvalue = 0;
	i = 1;
	while (argv[i] != NULL)
	{
		rvalue += env_del(env, argv[i]) != 0;
		i++;
	}
	return (rvalue);
}
