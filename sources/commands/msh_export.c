/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/06 22:39:40 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

// TO DO:
// If its reserved, it needs to go back to the appropriate slot
// WTFF export VAR+=suffix
uint8_t	env_add(t_env *env, const char *entry)
{
	const size_t	length = ft_strlen(entry) + 1;

	if (env->offset + length > FT_ENV_SIZE || env->count >= FT_ENV_ENTRIES - 1)
		return (1); // Out of memory
	env->ptr[env->count] = env->data + env->offset;
	ft_memcpy(env->ptr[env->count], entry, length);
	env->offset += length;
	env->count++;
	env->ptr[env->count] = NULL;
	return (0);
}

// TO DO: Add append and argless prints in
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
