/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:14:34 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/23 17:07:09 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 10:44:17 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/23 16:10:02 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "minishell.h"

// Copies env
uint8_t	init_env(t_shell *shell, t_memory *memory, const char * const *envp)
{
	size_t	i;
	size_t	length;
	size_t	offset;

	i = 0;
	offset = 0;
	while (envp[i] != NULL)
	{
		length = ft_strlen(envp[i]) + 1;
		if (length + offset > FT_ENV_SIZE || i >= FT_ENV_ENTRIES - 1)
			return (1);
		shell->envp[i] = ft_memcpy(memory->env_data + offset, envp[i], length);
		offset += length;
		i++;
	}
	shell->envp[i] = NULL;
	return (0);
}

static void	\
stt_initialize(t_shell *shell, t_memory *memory, const char **envp)
{
	shell->envp = memory->env_ptr;
}

int	main(int argc, const char **argv, const char **envp)
{
	t_memory	memory;
	t_shell		shell;

	stt_initialize(&shell, &memory, envp);
}
