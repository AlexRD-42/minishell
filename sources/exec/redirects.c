/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:12:50 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/12 15:35:44 by adeimlin         ###   ########.fr       */
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

// static
// t_argv	stt_initialize(char *word, char **ptr)
// {
// 	t_argv	arg;

// 	arg.data = word;
// 	arg.ptr = ptr;
// 	arg.count = 0;
// 	arg.offset = 0;
// 	arg.ptr[1] = NULL;
// 	return (arg);
// }

static int32_t
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

// 0: No problems, 1: dir function failure, 2: Ambiguous redirects, 4: Exceeded memory
static uint8_t
stt_open(t_token *token, t_env *env)
{
	uint8_t	rvalue;
	char	*ptr[2];
	char	word[256];
	t_argv	arg;

	rvalue = 0;
	arg = (t_argv){0, 0, word, NULL};
	rvalue = expand_token(token, env, &arg, 1);
	if (rvalue >= 2)
	{
		ft_write(2, "msh_redirects: Ambiguous or missing redirects\n", 47);
		return (2);
	}
	token->fd[0] = stt_parse_fd(token, ptr[0]);
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
