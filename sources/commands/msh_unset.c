/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 11:26:34 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

// 0  All name operands were successfully unset
// >0 At least one name could not be unset
int	msh_unset(t_vecp *argv, t_env *env)
{
	int		rvalue;
	size_t	i;

	if (argv->count < 2)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);	// Lack of arguments
	}
	rvalue = 0;
	i = 1;
	while (argv->ptr[i] != NULL)
	{
		rvalue += (env_del(argv->ptr[i], env) != 0);
		i++;
	}
	return (rvalue);
}
