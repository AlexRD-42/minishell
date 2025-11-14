/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:11:36 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/14 12:58:45 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_defines.h"
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdbool.h>
#include <minishell.h>

// var1="echo hello $VAR2"
// var1="echo hello "

// $var ls | > out0 | (<< EOF cat > out1) | << EOF cat | grep '$VAR' && << EOF2

// Things to watch out for:
// No commands in pipe region

// I should perform variable expansion as a first step, patching the token pointers;
// Left to right read until I find a pipe, henceforth named pipe region;
// Within a pipe region, the first word type (meaning not an operator) is the command to execute, 
// and all WORDS inside the pipe region that follow are arguments
// There may be multiple redirects, creating multiple files. Because its left to right the last one is the one thats in effect 
// Redirects breaks the pipeline in the sense that it will override what is being sent or received
// It is possible that one pipe region has no commands, but it may not be empty

// Parenthesis:
// cmd1 (cmd2 && (cmd3 || cmd4)):
// SH1 executes "cmd1" and sends "cmd2 && (cmd3 || cmd4)" to SH2, 
// SH2 executes "cmd2" and sends "cmd3 || cmd4" 			to SH3, 
// SH3 executes "cmd3 || cmd4"
// A syntactic unit should never have parenthesis inside

// $var ls | > out0 | wc > out1 "ahsdecho hello jkala" | << EOF cat | grep '$VAR'
// Token WORD: PTR *, 9
// Token WORD: PTR *, 20

// echo > out1 arg1 "arg2"
// echo arg1 arg2
// echo alex'dasds'$VAR
// alex WORD = alex
// dasds WORD | SINGLE QUOTE = dasds
// $VAR WORD = deimling

	// char	*argv[1024];			// 8kb
	// char	buffer[1024 * 1024];	// 1 mb

// This needs to expand variables as well as handle quotes
static
int	exec_error(int error_code)
{
	if (error_code == 1)
		write(2, "Pipex Error: Empty Path\n", 24);
	else if (error_code == 2)
		write(2, "Pipex Error: Path too long\n", 27);
	else
		perror("execve");
	return (-1);
}

static const
char	*find_path(const char **envp)
{
	const char	*ptr;

	if (envp == NULL)
		return (NULL);
	ptr = *envp;
	while (ptr != NULL)
	{
		if (ptr[0] == 'P' && ptr[1] == 'A' && ptr[2] == 'T' && ptr[3] == 'H'
			&& ptr[4] == '=')
			return (ptr + 5);
		ptr = *(envp++);
	}
	return (NULL);
}

// Attemps to execute the command through the different paths offered
// If it returns, then it failed to execute the command
// To do: change to exit instead of return
static
int	exec_cmd(char *cmd, char **argv, char **envp, size_t cmd_length)
{
	size_t			i;
	char			buffer[FT_PATH_SIZE];
	const char		*path = find_path(envp);
	int				error_code;

	error_code = 1;
	while (*path != 0)
	{
		i = 0;
		while (path[i] != 0 && path[i] != ':')
			i++;
		if (i + cmd_length >= 4094)
			return (2);
		if (i == 0)
			buffer[i++] = '.';
		else
			ft_memcpy(buffer, path, i);
		path += i + (path[i] == ':');
		buffer[i++] = '/';
		ft_memcpy(buffer + i, cmd, cmd_length);
		error_code = execve(buffer, argv, envp);
	}
	return (error_code);
}

// argv indexing needs double checking
// buffer doesnt get updated in globbing
int	msh_build_argv(t_token *token, t_env *env, t_argv *arg)
{
	size_t	i;
	char	pattern[FT_ARG_SIZE];

	i = 0;
	while ((token->type & E_CMD_END) == 0)
	{
		if (token->type == (E_WORD & E_EXPAND))
		{
			expand_word(token->ptr, token->length, env, pattern);
			i += expand_dir(pattern, argv + i, buffer);
		}
		else if (token->type == E_WORD)
		{
			argv[i++] = expand_word(token->ptr, token->length, env, buffer);
		}
		token++;
	}
	argv[i] = NULL;
	return (i);
}

// Procura primeira word:
int	pipe_exec(char *cmd, char **envp)
{
	char	*argv[FT_ARG_COUNT];
	size_t	i;
	size_t	cmd_length;
	int		error_code;

	i = 0;
	if (*cmd == 0)
	{
		argv[0] = cmd;
		argv[1] = NULL;
		return (exec_error(execve(cmd, argv, envp)));
	}
	cmd_length = in_place_split(cmd, argv) + 1;
	while (cmd[i] != 0 && cmd[i] != '/')
		i++;
	if (cmd[i] == '/')
		error_code = execve(cmd, argv, envp);
	else
		error_code = exec_cmd(cmd, argv, envp, cmd_length);
	return (exec_error(error_code));
}
