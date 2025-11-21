/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:49 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 11:24:23 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_utils.h"

// static
// int	stt_parse_str(const char *str, int64_t *number)
// {
// 	const char	*ostr = str;

// 	while (*str != 0)
// 	{
// 		if (*str < '0' || *str > '9')
// 			return (-1);
// 		str++;
// 	}
// 	if (str - ostr >= 18)
// 		return (-1);
// 	*number = ft_atoi(ostr);
// 	return (0);
// }

// prints exit always
int	msh_exit(t_vecp *argv, t_env *env)
{
	_exit(1);
}
