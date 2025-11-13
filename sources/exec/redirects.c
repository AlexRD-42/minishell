/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:12:50 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/13 10:50:08 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_types.h"

static int32_t	\
stt_parse_fd(t_token *token, char *str)
{
	int		fd;
	int32_t	flags;

	flags = O_RDONLY;
	if ((token->type & E_REDIR_IN) == 0)
	{
		if (token->type & E_APPND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else if (token->type & E_REDIR_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(str, flags, 0644);
	}
	else
		fd = open(str, flags);
	if (fd < 0)
		perror("msh_open: ");
	return (fd);
}

// Return: 0: Ok, 1: dir function, 2: Ambiguous redirects, 4: Out of memory
static uint8_t
stt_open(t_token *token, t_env *env)
{
	uint8_t	rvalue;
	char	word[256];

	rvalue = expand_token(token, env, &(t_argv){0, 0, word, NULL, word + sizeof(word)}, 1);
	if (rvalue >= 2)
	{
		ft_write(2, "msh_redirects: Ambiguous or missing redirects\n", 46);
		return (2);
	}
	token->fd[0] = stt_parse_fd(token, word);
	return (0);
}

void	open_files(t_token *tokens, t_env *env)
{
	while (tokens->type & E_STU_END)
	{
		if (tokens->type & (E_REDIR_IN | E_REDIR_OUT | E_APPND))
		{
				stt_open(tokens, env);
		}
		tokens++;
	}
}
