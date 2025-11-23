/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:55:18 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 19:52:39 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_utils.h"

// TO DO: Add append and argless prints in
// Parse string
static
int	stt_parse_str(const char *str)
{
	size_t	name_len;

	if (!((ft_ascii(*str) & E_LETTER) || *str == '_'))
		return (1);
	name_len = 1;
	while (ft_ascii(str[name_len]) & E_IDENT)
		name_len++;
	name_len += str[name_len] == '+';
	if (str[name_len] != '=')
		return (1);
	return (0);
}

int	msh_export(t_vecp *argv, t_env *env)
{
	size_t	i;
	int		rvalue;

	if (argv->count < 2)
		return (msh_env(env));
	rvalue = 0;
	i = 1;
	while (argv->ptr[i] != NULL)
	{
		if (stt_parse_str(argv->ptr[i]))
			rvalue = ft_error("msh_export: entry is not valid: ", argv->ptr[i], 1);
		else if (env_export(argv->ptr[i], env))
			rvalue = 1;
		i++;
	}
	return (rvalue);
}
