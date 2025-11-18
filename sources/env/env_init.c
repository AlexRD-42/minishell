/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:58:26 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 21:41:58 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"
#include "msh_test.h"
#include <stddef.h>
#include <stdint.h>

// Return: >= 0 on success
// Return: -1 OOM, 
// env.metadata could be a variable that contains info on blocks
ssize_t	envx_init(t_memory *mem, t_env *env, const char **envp)
{
	const char	*entry;
	size_t		length;

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
		length = ft_strlen(entry);
		if (envx_add((t_kstr){entry, length}, env))
			return (-1);
		env->count++;
	}
	env->ptr[env->count] = NULL;
	return ((ssize_t) env->count);
}
