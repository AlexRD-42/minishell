/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 15:24:01 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 18:31:54 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "minishell.h"
#include "msh_test.h"

static
char	*stt_assign_blocks(char *metadata, char *optr, size_t meta_index, t_wpair block)
{
	size_t	i;

	i = meta_index;
	metadata[i++] = 1;
	while (i < block.lower)
	{
		metadata[i] = 2;
		i++;
	}
	while (i < block.upper)
	{
		metadata[i] = 4;
		i++;
	}
	return (optr + meta_index * BLOCK_SIZE);
}

// To do:
static
ssize_t	stt_compact_blocks()
{
	
}

// Assign blocks, calls free if necessary
char	*request_blocks(t_env *env, size_t bytes)
{
	size_t			i;
	size_t			free_index;
	size_t			blocks_free;
	t_wpair			block;
	char			*metadata;

	block.lower = 1 + bytes / BLOCK_SIZE;
	block.upper = 4 * block.lower;
	metadata = env->metadata;
	while (block.upper >= block.lower)
	{
		i = 0;
		while (i < env->count)
		{
			blocks_free = 0;
			free_index = i;
			while (blocks_free < block.upper && metadata[i] == 0)
			{
				blocks_free++;
				i++;
			}
			if (blocks_free == block.upper)
				return (stt_assign_blocks(metadata, env->optr, free_index, block));
		}
		block.upper >>= 1;
	}
	// Call compact then retry
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	
}
