/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:55:44 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/22 14:21:16 by adeimlin         ###   ########.fr       */
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

int	msh_apply_redir(t_token *token, t_env *env)
{
	int	rvalue;
	int	fd;

	if (token->type & E_HRDOC)
		fd = token->fd[0];
	else
		fd = stt_open_file(token, env);
	if (fd < 0)
		return (-1);							// TODO: Meaningful returns
	if (token->type & (E_REDIR_IN | E_HRDOC))
		rvalue = dup2(fd, STDIN_FILENO);
	else
		rvalue = dup2(fd, STDOUT_FILENO);
	close(fd);
	if (rvalue == -1)
		return (ft_error("msh_dup2: ", NULL, -2));	// What happens on dup errors? do we continue?
	return (0);
}
