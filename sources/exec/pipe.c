/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 10:45:20 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/31 12:22:03 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <asm-generic/errno-base.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static
int	ft_child(char *cmd, int fd_read, int fd_write, char **envp)
{
	int	dup2_error;

	close(fd_read);
	dup2_error = dup2(fd_write, STDOUT_FILENO);
	close(fd_write);
	if (dup2_error == -1)
	{
		perror("dup2");
		exit(1);
	}
	pipe_exec(cmd, envp);
	exit(127);
}

static
int	ft_parent(int fd_read, int fd_write)
{
	int	dup2_error;

	close(fd_write);
	dup2_error = dup2(fd_read, STDIN_FILENO);
	close(fd_read);
	if (dup2_error == -1)
		perror("dup2");
	if (dup2_error == -1)
		return (-1);
	else
		return (1);
}

static
int	ft_pipe(char *cmd, char **envp, pid_t *cpid)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
		return (ft_child(cmd, fd[0], fd[1], envp));
	*cpid = pid;
	return (ft_parent(fd[0], fd[1]));
}

static
int	wait_child(pid_t *pid, size_t length)
{
	int	status;

	waitpid(pid[length], &status, 0);
	while (length > 0)
	{
		length--;
		if (pid[length] > 0)
			waitpid(pid[length], 0, 0);
	}
	return ((status & 0xff00) >> 8);
}

int	ft_pipe_loop(size_t argc, char **argv, char **envp, int *fd)
{
	size_t	i;
	pid_t	pid_list[FD_MAX];

	dup2(fd[0], STDIN_FILENO);
	i = 0;
	while (i < argc - 2)
	{
		ft_pipe(argv[i], envp, pid_list + i);
		i++;
	}
	pid_list[i] = fork();
	if (pid_list[i] < 0)
	{
		perror("fork");
		close(fd[1]);
		return (1);
	}
	if (pid_list[i] == 0)
		ft_child(argv[argc - 2], fd[0], fd[1], envp);
	close(fd[0]);
	close(fd[1]);
	close(STDIN_FILENO);
	return (wait_child(pid_list, i));
}
