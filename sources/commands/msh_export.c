/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/19 12:07:26 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

// -1) OOM
// V lower V upper (length)
// variable=value
// Check without =
// int	mshx_export(const char *entry, t_env *env)
// {
	// size_t	length;
	// size_t	index;
	// bool	append;

	// length = 0;
	// while (entry[length] != 0 && entry[length] != '=')
	// 	length++;
	// append = (length > 1) && (entry[length] == '=')	&& entry[length - 1] == '+';
	// env_find(entry, length - append, &index, env);
	// length += (entry[length] == '=');
	// return (env_add((t_kstr){entry, length}, index, append, env));
//}

// // TO DO: Add append and argless prints in
// int	msh_export(int argc, const char **argv, t_env *env)
// {
// 	size_t	i;
// 	int		rvalue;

// 	if (argc < 2)
// 	{
// 		write(STDOUT_FILENO, "\n", 1);
// 		return (1);	// Lack of arguments
// 	}
// 	rvalue = 0;
// 	i = 1;
// 	while (argv[i] != NULL)
// 	{
// 		rvalue += env_add(env, argv[i]) != 0;
// 		i++;
// 	}
// 	return (rvalue);
// }
