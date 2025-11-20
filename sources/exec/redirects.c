/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 11:12:50 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/20 14:00:01 by adeimlin         ###   ########.fr       */
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
#include "msh_utils.h"

static int32_t	\
stt_parse_fd(const uint32_t type, const char *str)
{
	int		fd;
	int32_t	flags;

	flags = O_RDONLY;
	if ((type & E_REDIR_IN) == 0)
	{
		if (type & E_APPND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else if (type & E_REDIR_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		fd = open(str, flags, 0644);
	}
	else
		fd = open(str, flags);
	if (fd < 0)
		perror("msh_open: ");
	return (fd);
}

// Return: >=0: Ok, -1) OOM, -2) dir function, -4) Ambiguous redirects
static ssize_t
stt_open(t_token *token, t_env *env)
{
	ssize_t		rvalue;
	char	word[256];
	char	*ptr[1];
	t_buf	buf;

	buf = (t_buf){word, word + sizeof(word), word};
	rvalue = expand_token(token, env, &(t_vecp){buf, 0, 1, ptr});
	if (rvalue >= 0)
		token->fd[0] = stt_parse_fd(token->type, word);
	else if (rvalue == -4)
		return (ft_error("msh_redirects: Ambiguous redirects", "", rvalue));
	return (rvalue);
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
