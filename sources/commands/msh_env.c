/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/06 22:26:57 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_utils.h"

// // Running env with no arguments will print the env to stdout
// // TO DO: option to print in ascending order
// int	msh_env(int argc, const char **argv, t_env *env)
// {
// 	size_t	i;
// 	size_t	length;
// 	size_t	offset;
// 	uint8_t	buffer[FT_ENV_SIZE];

// 	(void) argc;
// 	(void) argv;
// 	i = 0;
// 	offset = 0;
// 	while (env->ptr[i] != NULL)
// 	{
// 		length = ft_strlen(env->ptr[i]);
// 		ft_memcpy(buffer + offset, env->ptr[i], length);
// 		offset += length;
// 		buffer[offset++] = '\n';
// 		i++;
// 	}
// 	return (ft_write(STDOUT_FILENO, buffer, offset) <= 0);
// }
