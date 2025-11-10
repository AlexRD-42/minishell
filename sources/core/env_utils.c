/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:47:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/10 12:37:39 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"

// Attempts to find the entry string in Env vars
// Searches backwards to delete reserved vars last
size_t	env_find(t_env *env, const char *entry, size_t length)
{
	size_t	i;
	size_t	j;

	if (length == 0)
	{
		while (entry[length] != 0 && entry[length] != '=')
			length++;
	}
	i = env->count + (env->count == 0);
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

uint8_t	env_del(t_env *env, const char *entry)
{
	char	*ptr;
	size_t	index;
	size_t	length;

	index = env_find(env, entry, 0);
	if (index == SIZE_MAX)
		return (1); // Could not find var
	if (index <= 3)
	{
		ft_memset(env->ptr[index], 0, FT_PATH_MAX);
		return (0);
	}
	ptr = env->ptr[index];
	length = ft_strlen(ptr) + 1;
	env->offset -= length;
	ft_memmove(ptr, ptr + length, env->offset - (size_t)(ptr - env->data));
	while (index < env->count - 1)
	{
		env->ptr[index] = env->ptr[index + 1] - length;
		index++;
	}
	env->ptr[index] = NULL;
	env->count--;
	return (0);
}

// first 16kb are reserved for PATH, PWD, OLDPWD and TBD
// What if envp is null?
static
uint8_t	env_copy(t_env *env, const char **envp_src)
{
	size_t	i;
	size_t	length;

	ft_memset(env->data, 0, FT_ENV_SIZE);
	env->ptr[0] = env->data;
	env->ptr[1] = env->ptr[0] + FT_PATH_MAX;
	env->ptr[2] = env->ptr[1] + FT_PATH_MAX;
	env->ptr[3] = env->ptr[2] + FT_PATH_MAX;
	env->offset = 4 * FT_PATH_MAX;
	i = 4;
	while (envp_src[i] != NULL)
	{
		length = ft_strlen(envp_src[i]) + 1;
		if (length + env->offset > FT_ENV_SIZE || i >= FT_ENV_ENTRIES - 1)
			return (1);	// Out of memory
		env->ptr[i] = env->data + env->offset;
		ft_memcpy(env->ptr[i], envp_src[i], length);
		env->offset += length;
		i++;
	}
	env->count = i;
	env->ptr[i] = NULL;
	return (0);
}

// Copies an env to the arena in t_env, and moves path, pwd, oldpwd
// to the first 16kb (reserved for them)
// Needs to increment SHLVL (just call export later)
uint8_t	env_init(t_env *env, const char **envp_src)
{
	size_t	i;

	if (envp_src == NULL)
		return (1);
	env_copy(env, envp_src);
	i = env_find(env, "PATH", 4);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[0], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	i = env_find(env, "PWD", 3);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[1], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	i = env_find(env, "OLDPWD", 6);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[2], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	// Need to implement export properly for SHLVL
	env_del(env, "PATH");
	env_del(env, "PWD");
	env_del(env, "OLDPWD");
	return (0);
}

int	main(int argc, const char **argv, const char **envp)
{
	t_shell	shell;
	
	env_copy(&shell.env, envp);
}
