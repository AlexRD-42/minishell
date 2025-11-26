/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:55:44 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/26 16:41:41 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "minishell.h"
#include "msh_defines.h"
#include "msh_utils.h"
#include "msh_types.h"
#include <stdio.h>

// Return: >=0: Ok, -1) Open failure (P)
static
int32_t	stt_parse_fd(const uint32_t type, const char *str)
{
	int		fd;

	fd = -1;
	if (type & E_REDIR_IN)
		fd = open(str, O_RDONLY);
	else if (type & E_APPND)
		fd = open(str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type & E_REDIR_OUT)
		fd = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	return (rvalue);
}

// Return: >=0: Ok, -1) OOM (P), -2) dir function (P), -4) Ambiguous redir (P)
// Overrides the previous fd and closes
static
int	stt_override_fd(t_token *token, t_env *env, int *fd)
{
	const bool	target = !!(token->type & (E_REDIR_OUT | E_APPND));
	const int	old_fd = fd[target];
	int			new_fd;

	if (token->type & E_HRDOC)
		new_fd = token->fd[0];
	else
		new_fd = stt_open_file(token, env);
	if (new_fd >= 0)
	{
		if (old_fd >= 0)
			close(old_fd);
		fd[target] = new_fd;
	}
	return (new_fd);
}

// Returns: 0) OK, -1) Fatal Error (P)
int	msh_open_files(t_token *tokens, t_token *end, t_env *env, int *fd)
{
	ssize_t		pdepth;
	uint32_t	type;

	pdepth = 0;
	while (tokens < end && !(tokens->type & E_END))
	{
		type = tokens->type;
		pdepth += !!(type & E_OPAREN) - !!(type & E_CPAREN);
		if (pdepth == 0 && (type & E_REDIR))
		{
			if (stt_override_fd(tokens, env, fd) == -1)
				return (-1);	// Proceed on all other errors
		}
		if (pdepth < 0)
			return (ft_error("msh_unexpected: Negative pdepth", "", -1));
		tokens++;
	}
	return (0);
}
