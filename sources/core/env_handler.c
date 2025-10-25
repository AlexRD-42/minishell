/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:47:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/25 14:03:26 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

int	env_print(t_env *env)
{
	ssize_t	rvalue;
	size_t	i;
	size_t	length;

	if (env->count == 0 || env->ptr[0] == NULL)
		return (1);
	i = 1;
	while (i < env->count)
	{
		env->ptr[i][-1] = '\n';
		i++;
	}
	length = ft_strlen(env->ptr[env->count - 1]);
	env->ptr[env->count - 1][length] = '\n';
	rvalue = ft_write(STDOUT_FILENO, env->ptr[0], env->offset);
	env->ptr[env->count - 1][length] = 0;
	i = 1;
	while (i < env->count)
	{
		env->ptr[i][-1] = '\n';
		i++;
	}
	return (rvalue <= 0);	// 1 on failure
}

// Guarantee that str does not have '='
size_t	env_find(t_env *env, const char *str)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < env->count)
	{
		j = 0;
		while (env->ptr[i][j] == str[j] && str[j] != 0)
			j++;
		if (str[j] == 0 && env->ptr[i][j] == '=')
			return (i);
		i++;
	}
	return (SIZE_MAX);
}

// Optionally make it so that compaction happens on threshold of free space
uint8_t	env_del(t_env *env, const char *entry)
{
	char	*ptr;
	size_t	index;
	size_t	length;

	index = env_find(env, entry);
	if (index == SIZE_MAX)
		return (1); // Could not find var
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

// Need to reserve first 8kb to PWD and OLDPWD
uint8_t	env_copy(t_env *env, const char **envp_src)
{
	size_t	i;
	size_t	length;

	i = 0;
	env->offset = 0;
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

int	main(int argc, const char **argv, const char **envp)
{
	t_shell	shell;
	
	env_copy(&shell.env, envp);
	env_print(&shell.env);
}
