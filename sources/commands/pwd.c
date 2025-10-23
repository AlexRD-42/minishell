/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:26:54 by adeimlin          #+#    #+#             */
/*   Updated: 2025/10/23 15:41:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
pwd - prints the name of current/working directory
uses getcwd to determine the current path
*/

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

// Using getcwd might introduce mallocs to the code
// To do: error handling (stt function maybe)
// Check if cwd returns nl
// Check short writes

int	msh_pwd(int argc, const char **argv)
{
	char	buffer[FT_PATH_MAX];

	(void) argc;
	(void) argv;
	if (getcwd(buffer, FT_PATH_MAX) == NULL)
	{
		perror("cwd");
		return (1);
	}
	if (write(STDOUT_FILENO, buffer, ft_strlen(buffer)) < 0)
	{
		perror("pwd");
		return (1);
	}
	write(STDOUT_FILENO, "\n", 1);
	return (0);
}
