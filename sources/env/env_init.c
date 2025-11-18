/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:58:26 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 17:42:25 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include "msh_test.h"
#include <stddef.h>
#include <stdint.h>

// If length is size_max, it will be calculated
// The length return is the envp index
char	*envx_find(t_env *env, const char *entry, size_t length, size_t *index)
{
	size_t	i;
	size_t	j;

	if (length == SIZE_MAX)
	{
		length = 0;
		while (ft_ascii(entry[length]) & E_IDENT)
				length++;
	}
	if (length == 0)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		j = 0;
		while (j < length && env->ptr[i][j] == entry[j])
			j++;
		if (j == length && env->ptr[i][j] == '=')
		{
			if (index != NULL)
				*index = i;
			return (env->ptr[i]);
		}
		i++;
	}
	return (NULL);
}

// Return: >= 0 on success
// Return: -1 OOM, 
// env.metadata could be a variable that contains info on blocks
ssize_t	envx_init(t_memory *mem, t_env *env, const char **envp)
{
	const char	*entry;

	env->metadata = mem->metadata;
	env->optr = mem->env_block;
	env->count = 0;
	env->max_count = FT_ENV_COUNT;
	env->ptr = mem->env_ptr;
	ft_memset(env->optr, 0, FT_ENV_SIZE);
	ft_memset(env->metadata, 0, FT_ENV_COUNT);	// Careful
	while (envp[env->count] != NULL)	// no guard for env->count, env_add responsability
	{
		entry = envp[env->count];
		if (envx_add(env, entry, 0))
			return (-1);
		env->count++;
	}
	env->ptr[env->count] = NULL;
	return ((ssize_t) env->count);
}
