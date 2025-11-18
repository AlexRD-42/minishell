/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:40:45 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/16 19:45:07 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include "minishell.h"

// first 16kb are reserved for PATH, PWD, OLDPWD and TBD
// What if envp is null?
static
uint8_t	env_copy(t_vecp *env, const char **envp_src)
{
	size_t	i;
	size_t	length;

	ft_memset(env->optr, 0, FT_ENV_SIZE);
	env->ptr[0] = env->optr;
	env->ptr[1] = env->ptr[0] + FT_PATH_SIZE;
	env->ptr[2] = env->ptr[1] + FT_PATH_SIZE;
	env->ptr[3] = env->ptr[2] + FT_PATH_SIZE;
	env->wptr = env->ptr[3] + FT_PATH_SIZE;
	i = 4;
	while (envp_src[i] != NULL)
	{
		length = ft_strlen(envp_src[i]) + 1;
		if (env->wptr + length > env->end || i >= env->max_count - 1)
			return (1);	// Out of memory
		env->ptr[i] = env->wptr;
		ft_memcpy(env->ptr[i], envp_src[i], length);
		env->wptr += length;
		i++;
	}
	env->count = i;
	env->ptr[i] = NULL;
	return (0);
}

// Copies an env to the arena in t_env, and moves path, pwd, oldpwd
// to the first 16kb (reserved for them)
// Needs to increment SHLVL (just call export later)
// Length check?
uint8_t	env_init(t_vecp *env, const char **envp_src)
{
	size_t	i;

	if (envp_src == NULL)
		return (1);
	env_copy(env, envp_src);
	i = env_find(env, "PATH", 4);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[E_PATH], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	i = env_find(env, "PWD", 3);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[E_PWD], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	i = env_find(env, "OLDPWD", 6);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[E_OLDPWD], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	// Need to implement export properly for SHLVL
	env_del(env, "PATH");
	env_del(env, "PWD");
	env_del(env, "OLDPWD");
	return (0);
}