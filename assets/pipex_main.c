/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 11:23:48 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/03 15:13:31 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

static
void	pipex_init(char **argv, int argc, int *fd, uint8_t is_here_doc)
{
	if (is_here_doc == 1)
		fd[0] = here_doc(argv[2]);
	else
		fd[0] = open(argv[1], O_RDONLY, 0644);
	if (fd[0] == -1)
	{
		perror("open");
		fd[0] = open("/dev/null", O_RDONLY);
	}
	if (is_here_doc)
		fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd[1] = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd[1] == -1)
	{
		close(fd[0]);
		perror("open");
	}
}

int	main(int argc, char **argv, char **envp)
{
	int				fd[2];
	const uint8_t	is_here_doc = ft_strcmp(argv[argc != 1], "here_doc") == 0;
	const size_t	cmd_count = (size_t)(argc - 2 - is_here_doc);
	int				exit_code;

	if (argc <= (4 + is_here_doc))
	{
		write(2, "Pipex: Incorrect argument count\n", 32);
		return (-1);
	}
	pipex_init(argv, argc, fd, is_here_doc);
	exit_code = ft_pipe_loop(cmd_count, argv + 2 + is_here_doc, envp, fd);
	return (exit_code);
}
