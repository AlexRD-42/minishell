/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_idea.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:58:26 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/17 17:06:13 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "msh_defines.h"
#include "msh_types.h"

size_t	envx_find(t_vecp *env, const char *entry)
{
	size_t	i;
	size_t	j;
	size_t length;

	length = 0;
	while (ft_ascii(entry[length]) & E_IDENT)
			length++;
	if (length == 0)
		return (SIZE_MAX);
	i = env->count;
	while (i-- > 0)
	{
		j = 0;
		while (j < length && env->ptr[i][j] == entry[j])
			j++;
		if (j == length && env->ptr[i][j] == '=')
			return (i);
	}
	return (SIZE_MAX);
}

// Return: >= 0 on success
// Return: -1 OOM,
// env.metadata could be a variable that contains info on blocks
ssize_t	envx_init(t_memory *mem, t_vecp *env, const char **envp)
{
	const char	*entry;

	env->buf = (t_buf){mem->env_block, mem->env_block + FT_ENV_SIZE, mem->env_block};
	env->count = 0;
	env->max_count = FT_ENV_COUNT;
	env->ptr = mem->env_ptr;
	ft_memset(env->optr, 0, FT_ENV_SIZE);
	while (envp[env->count] != NULL)	// no guard for env->count, env_add responsability
	{
		entry = envp[env->count];
		if (envx_add(env, entry))
			return (-1);
		env->count++;
	}
	env->ptr[env->count] = NULL;
	return ((ssize_t) env->count);
}

// Searches for env var equal to entry
// If a match was found, checks space remaining by looping through the sentinels
// if (free_space > entry length), we just overwrite
// else we mark for deletion and search for next block big enough
// If no blocks are available, we call compact. If still not enough, OOM
// Same rules for append, except we don't overwrite
envx_add(const char *entry)
{
}

envx_del(const char *entry)
{
	
}