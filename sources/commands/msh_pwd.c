/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/18 10:41:38 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

/* pwd - prints the name of current/working directory
uses getcwd to determine the current path
Using getcwd might introduce mallocs to the code */
int	msh_pwd(void)
{
	char	buffer[FT_PATH_SIZE];
	size_t	length;

	if (getcwd(buffer, FT_PATH_SIZE) == NULL)
	{
		perror("cwd");
		return (1);
	}
	length = ft_strlen(buffer);
	buffer[length] = '\n';
	if (ft_write(STDOUT_FILENO, buffer, length + 1) < 0)
		return (1);
	return (0);
}
