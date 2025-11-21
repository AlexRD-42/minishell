/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:55:44 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 21:01:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <fcntl.h>
#include "minishell.h"
#include "msh_utils.h"
#include "msh_types.h"

// Return: >=0: Ok, -1) Open failure (P)
static
int32_t	stt_parse_fd(const uint32_t type, const char *str)
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
		return (ft_error("msh_open: ", NULL, -1));
	return (fd);
}

// Return: >=0: Ok, -1) OOM (P), -2) dir function (P), -4) Ambiguous redir (P)
static
int32_t	stt_open_file(t_token *token, t_env *env)
{
	ssize_t		rvalue;
	char		word[256];
	char		*ptr[1];
	t_buf		buf;

	buf = (t_buf){word, word + sizeof(word), word};
	rvalue = expand_token(token[1], env, &(t_vecp){buf, 0, 1, ptr});
	if (rvalue >= 0)
		return (stt_parse_fd(token->type, word));
	if (rvalue == -4)
		ft_error("msh_redirects: Ambiguous redirects", "", rvalue);
	return (-1);
}

static
int	stt_apply_redir(t_token *token, t_env *env)
{
	int	fd;
	int	target_fd;

	if (token->type & E_HRDOC)
	{
		fd = token->fd[0];
		if (fd < 0)
			return (-1);
		target_fd = STDIN_FILENO;
	}
	else
	{
		fd = stt_open_file(token, env);
		if (fd < 0)
			return (-1);
		if (token->type & (E_REDIR_IN))
			target_fd = STDIN_FILENO;
		else
			target_fd = STDOUT_FILENO;
	}
	if (dup2(fd, target_fd) < 0)
		return (ft_error("msh_dup2: ", NULL, -1));
	close(fd);
	return (0);
}

// Need to open everything up until E_AND, E_OR, E_PIPE is found
// When a () is found, find its closing pair ignoring everything else
// Then find the next interval until a pipe or E_AND, E_OR, E_PIPE is found
// end is important for subshells
ssize_t	msh_open_files(t_token *start, t_token *end, t_env *env)
{
	ssize_t		count;
	ssize_t		pdepth;
	uint32_t	type;
	t_token		*tokens;

	tokens = start;
	count = 0;
	pdepth = 0;
	while (tokens < end && (tokens->type & (E_OPERATOR | E_END)) == 0)
	{
		type = tokens->type;
		pdepth += !!(type & E_OPEN_PAREN) - !!(type & E_CLOSE_PAREN);
		if (type & E_REDIR && pdepth == 0)
		{
			if (stt_apply_redir(tokens, env) < 0)
				return (-1);
			count++;
		}
		if (pdepth < 0)
			ft_error("Houston we have a problem", "", -1);
		tokens++;
	}
	return (count);
}
