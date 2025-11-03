/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:47:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/03 16:48:45 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"

// Guarantee that str does not have '='
size_t	env_find(t_env *env, const char *entry, size_t start)
{
	size_t	i;
	size_t	j;

	i = start;
	while (i < env->count)
	{
		j = 0;
		while (env->ptr[i][j] == entry[j] && entry[j] != 0)
			j++;
		if (entry[j] == 0 && env->ptr[i][j] == '=')
			return (i);
		i++;
	}
	return (SIZE_MAX);
}

// Optionally make it so that compaction happens on lack of free space
size_t	env_del(t_env *env, const char *entry, size_t start)
{
	char	*ptr;
	size_t	index;
	size_t	length;

	index = env_find(env, entry, start);
	if (index == SIZE_MAX)
		return (SIZE_MAX); // Could not find var
	if (index <= 3)
	{
		ft_memset(env->ptr[index], 0, FT_PATH_MAX);
		return (index);
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
	return (index);
}

// TO DO:
// If its reserved, it needs to go back to the appropriate slot
// WTFF export VAR+=suffix
uint8_t	env_add(t_env *env, const char *entry)
{
	const size_t	length = ft_strlen(entry) + 1;

	if (env->offset + length > FT_ENV_SIZE || env->count >= FT_ENV_ENTRIES - 1)
		return (1); // Out of memory
	env->ptr[env->count] = env->data + env->offset;
	ft_memcpy(env->ptr[env->count], entry, length);
	env->offset += length;
	env->count++;
	env->ptr[env->count] = NULL;
	return (0);
}

// first 16kb are reserved for PATH, PWD, OLDPWD and TBD
// What if envp is null?
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
uint8_t	env_init(t_env *env, const char **envp_src)
{
	size_t	i;

	if (envp_src == NULL)
		return (1);
	env_copy(env, envp_src);
	i = env_find(env, "PATH", 0);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[0], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	i = env_find(env, "PWD", 0);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[1], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	i = env_find(env, "OLDPWD", 0);
	if (i != SIZE_MAX)
		ft_memcpy(env->ptr[2], env->ptr[i], ft_strlen(env->ptr[i]) + 1);
	env_del(env, "PATH", 4);
	env_del(env, "PWD", 4);
	env_del(env, "OLDPWD", 4);
	return (0);
}

int	main(int argc, const char **argv, const char **envp)
{
	t_shell	shell;
	
	env_copy(&shell.env, envp);
}
