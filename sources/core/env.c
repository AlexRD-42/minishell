/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:47:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/24 16:05:34 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

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
	ft_memcpy(ptr, ptr + length, env->offset - (size_t)(ptr - env->data));
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

uint8_t	env_copy(t_env *env, const char * const *envp_src)
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

	
}
