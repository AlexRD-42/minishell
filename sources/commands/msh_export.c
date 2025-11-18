/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 10:42:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

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
