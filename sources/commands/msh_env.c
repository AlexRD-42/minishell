/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/03 16:49:23 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

// Running env with no arguments will print the env to stdout
int	msh_env(int argc, const char **argv, t_env *env)
{
	size_t	i;
	size_t	length;
	size_t	offset;
	uint8_t	buffer[FT_ENV_SIZE];

	(void) argc;
	(void) argv;
	i = 0;
	offset = 0;
	while (env->ptr[i] != NULL)
	{
		length = ft_strlen(env->ptr[i]);
		ft_memcpy(buffer + offset, env->ptr[i], length);
		offset += length;
		buffer[offset++] = '\n';
		i++;
	}
	return (ft_write(STDOUT_FILENO, buffer, offset) <= 0);
}

//
int	msh_export(int argc, const char **argv, t_env *env)
{
	size_t	i;
	int		rvalue;

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
		rvalue += (env_del(env, argv[i], 0) != SIZE_MAX);
		i++;
	}
	return (rvalue);
}
