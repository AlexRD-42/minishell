/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 13:54:25 by adeimlin          #+#    #+#             */
/*   Updated: 2025/11/23 19:07:42 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/limits.h>
#include <unistd.h>
#include <stdio.h>
#include "minishell.h"
#include "msh_utils.h"

/* pwd - prints the name of current/working directory
uses getcwd to determine the current path
Using getcwd might introduce mallocs to the code */
int	msh_pwd(void)
{
	char	buffer[FT_PATH_SIZE];
	size_t	length;

	if (getcwd(buffer, FT_PATH_SIZE) == NULL)
		return (ft_error("msh_cwd: ", NULL, 1));
	length = ft_strlen(buffer);
	buffer[length] = '\n';
	if (ft_write(STDOUT_FILENO, buffer, length + 1) < 0)
		return (1);
	return (0);
}
