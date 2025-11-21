/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_stu.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 10:14:41 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/21 16:03:01 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "minishell.h"
#include "msh_types.h"
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include "msh_utils.h"
#include <stdlib.h>
#include <signal.h>

int	open_and_dup(t_token *curr, t_token *end, t_env *env);

// This is wrong, the loop should stop upon finding first open paren
static
t_token	*stt_next_sep(t_token *tokens)
{
	int	in_paren;

	in_paren = 0;
	while (!(tokens->type & (E_STU_END | E_END)))
	{
		if (tokens->type & E_OPEN_PAREN)
			in_paren++;
		else if (tokens->type & E_CLOSE_PAREN)
			in_paren--;
		if (!in_paren && (tokens->type & E_PIPE))
			return (tokens);
		tokens++;
	}
	return (tokens);
}

static
void	stt_child(t_token *current, t_token *end, t_env *env, int fds[3])
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (fds[0] != -1)
	{
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	if (fds[1] != -1)
	{
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[2]);
	}
	if (open_and_dup(current, end, env) < 0)
		exit(1);
	end->type |= E_CMD_END;			// Check
	// if (current->type & E_OPEN_PAREN)
	// {
	// 	prepare_subshell(current);
	// 	_exit(non_interactive_shell(current + 1, env));
	// }
	exec_pipe(current, env);
}

static
int wait_all_children(pid_t *pids, int count)
{
    int i;
    int status;
    int last_status;

    i = 0;
    last_status = 0;
    while (i < count)
    {
        waitpid(pids[i], &status, 0);
        if (i == count - 1)
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
        i++;
    }
    return (last_status);
}

static
int stt_exec_builtin_parent(t_token *tokens, t_env *env)
{
    char    *arg_ptr[FT_ARG_COUNT];
    char    buffer[FT_ARG_SIZE];
    t_vecp  argv;
    int     saved_stdin;
    int     saved_stdout;
    int     exit_code;

    argv = (t_vecp){{buffer, buffer + sizeof(buffer), buffer}, 0, FT_ARG_COUNT, arg_ptr};
    if (msh_build_argv(tokens, env, &argv) < 0)
        return (1);
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
        return (perror("dup"), 1);
    if (open_and_dup(tokens, tokens + argv.count, env) < 0)
        exit_code = 1;
    else
        exit_code = msh_dispatch(&argv, env);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    return (exit_code);
}

// First thing exec_stu does is: 
int	exec_stu(t_token *tokens, t_env *env)
{
	t_token	*end;
	int		fd[3];
	int		prev_read;
	pid_t	pids[32];		// Standardize
	int		status;
	int		i;

	i = 0;
	status = 0;
	prev_read = -1;
	while (!(tokens->type & (E_STU_END)))
	{
		end = stt_next_sep(tokens);
		if (!(end->type & E_PIPE) && tokens->type & E_WORD && msh_mutates_state(tokens->ptr))
        {
            status = stt_exec_builtin_parent(tokens, env);
            tokens = end + 1;
            break ;
        }
		if ((end->type & E_PIPE) && pipe(fd) == -1)
			return (ft_error("msh", "", 1));
		pids[i] = fork();
		if (pids[i] == 0)
		{
			if (!(end->type & E_PIPE))
				fd[1] = -1;
			stt_child(tokens, end, env, (int [3]){prev_read, fd[1], fd[0]});
		}
		if (prev_read != -1)
			close(prev_read);
		if (end->type & E_PIPE)
		{
			close(fd[1]);
			prev_read = fd[0];
		}
		if (!(end->type & E_STU_END))
			tokens = end + 1;
		else
			tokens = end;
		i++;
	}
	if (prev_read != -1)
		close(prev_read);
	if (i > 0)
		status = wait_all_children(pids, i);
	return (status);
}
